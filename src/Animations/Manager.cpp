#include "Manager.h"

using namespace Animations;

Animation* Manager::createAnimation(CharacterOnBoard* animatedObj,
                                    Character::Activity activity,
                                    Direction direction)
{
  return new Animations::OnTexture(animatedObj, activity , direction);
}

Animation* Manager::createAnimation(CharacterOnBoard* animatedObj,
                                    Character::Activity activity)
{
  auto direction = animatedObj->getDirection();

  return new Animations::OnTexture(animatedObj, activity , direction);
}

Animation* Manager::createAnimationMove(CharacterOnBoard* animatedObj,
                                        Animations::Actions::Move move)
{
  return new Animations::MoveBy(animatedObj, move);
}

Animation* Manager::createAnimationHurt(CharacterOnBoard* animatedObj,
                                        int damage)
{
  return new Animations::Hurt(animatedObj, damage);
}

Animation* Manager::createAnimationDeath(CharacterOnBoard* animatedObj)
{
  return new Animations::Death(animatedObj);
}

Animation* Manager::createAnimationDisappear(CharacterOnBoard* animatedObj)
{
  return new Animations::Disappear(animatedObj, animatedObj->getActivity(), animatedObj->getDirection());
}

std::vector<Animation*>& Manager::addNewSet()
{
  // utworzenie nowej, pustej ścieżki animacji
  std::vector<Animation*> new_set;
  sets_queue.push(new_set);

  return sets_queue.back();
}

void Manager::addAnimationToSet(std::vector<Animation*>& set, Animation* animation)
{
  set.push_back(animation);
}

void Manager::addAnimationToQueue(Animation* animation)
{
  // utworzenie nowego pustego zbioru na animacje
  std::vector<Animation*>& set = addNewSet();

  // wrzucenie tam animacji
  addAnimationToSet(set, animation);
}

void Manager::addIdleAnimation(Animation* animation)
{
  idle_animations.push_back(animation);
}

bool Manager::update_animation_and_check_if_finished(Animation& animation,
                                                     float delta)
{
  animation.animate(delta);

  auto object = animation.animatedObj;
  auto & sprites_data = object->get_current_sprites_data();

  // zwiększenie numeru klatki dla animowanego obiektu (dla klas animujących teksturę, tj. pochodnych OnTexture)
  if (dynamic_cast<OnTexture*>(&animation))
    current_frame[object] += delta * sprites_data.fps;

  // info, że animacja wykonała się do ostatniej klatki
  bool loop_reseted = false;

  // utrzymanie wartości licznika animacji w jej zakresie
  while (current_frame[object] >= sprites_data.textures.size() )
    {
      if (animation.finish_condition == Animations::FinishCondition::to_last_frame)
        current_frame[object] = sprites_data.textures.size() - 1;
      else
        current_frame[object] -= sprites_data.textures.size();

      loop_reseted = true;
    }

  // ustawienie tekstury na zaokrąglony do int numer obecnej klatki tego obiektu
  object->setTexture(sprites_data.textures[static_cast<int>(current_frame[object])]);

  // sprawdzanie czy spełniony został warunek kończący animację
  switch (animation.finish_condition) {
  case FinishCondition::no_repeat: {
    if (loop_reseted) {
      object->reset_texture();
      return true;
    }
  } break;
  case FinishCondition::to_last_frame: {
    if (loop_reseted)
      return true;
  } break;
  case FinishCondition::special: {
    if (animation.special_finish_condition_obtained()) {
      object->reset_texture();
      return true;
    }
  } break;
  }
  return false;
}

void Manager::updateAnimationsStack(float delta)
{
  // jeżeli obecny zbiór animacji jest pusty, pobranie kolejnego z kolejki o ile jakieś zostały
  if (current_set.empty() && !sets_queue.empty()) {
    // zdjęcie oczekującego zbioru animacji z kolejki
    current_set = sets_queue.front();
    sets_queue.pop();

    // i uruchomienie wszystkich animacji w nim
    for (auto & animation : current_set) {
      // wychwycenie wyjątku w przypadku próby uruchomienia animacji która nie jest zdefiniowana
      try {
        animation->init();
      }
      catch (errors::no_anim_for_activity) {
        // w przeciwnym razie program zawiesiłby się
        std::cout << "Error! Animation for runned action does't exist.\n";
        std::exit(1);
      }

      // ustawienie animacji na początkową klatkę (tylko dla animacji z ustawionym isResettingFrame)
      if (animation->isResettingFrame) {
        current_frame[animation->animatedObj] = 0;
      }
    }
  }

  std::vector<Animation*> animationsToErase;

  // wywołanie wszystkich kolejnych animacji z danego zbioru równocześnie
  for (auto & an : current_set) {
    if (update_animation_and_check_if_finished(*an, delta)) {
      // dodanie zakończonej animacji do usunięcia za chwilę
      animationsToErase.push_back(an);
    }
  }

  // usunięcie zakończonych animacji ze zbioru i z pamięci
  for (auto & an : animationsToErase)
    for(std::vector<Animation*>::iterator it = current_set.begin(); it != current_set.end(); it++) {
      if (*it == an) {
        current_set.erase(it);
        delete an;
        break;
      }
    }
}

void Manager::updateIdleAnimations(std::vector<CharacterOnBoard*> characters, float delta)
{
  // zmiana statusu na idle wszystkim postaciom, dla których nie odgrywa się żadna animacja w tej chwili
  for (auto & character : characters) {
    bool any_animation_on_it = false;
    for (auto & animation : current_set) {
      // sprawdzenie czy jest obecnie jakaś animacja na tej postaci
      if (animation->animatedObj == character) {
        any_animation_on_it = true;
        break;
      }
    }
    if (!any_animation_on_it)
      character->setActivity(Character::Activity::idle);
  }

  // update wszystkich animacji bezczynności
  for(auto & ani : idle_animations) {
    if (ani->animatedObj->getActivity() == Character::Activity::idle)
      update_animation_and_check_if_finished(*ani, delta);
  }
}

bool Manager::anyAnimationLocking()
{
  if (sets_queue.empty() && current_set.empty())
    return false;
  return true;
}
