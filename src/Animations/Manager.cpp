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
                                        Animations::Actions::Move move,
                                        bool isBlocking)
{
  return new Animations::MoveBy(animatedObj, move, isBlocking);
}

Animation* Manager::createAnimationHurt(CharacterOnBoard* animatedObj)
{
  return new Animations::Hurt(animatedObj);
}

Animation* Manager::createAnimationTakeDamageHP(CharacterOnBoard* animatedObj,
                                                int damage)
{
  return new Animations::ChangeHpBar(animatedObj, damage);
}

Animation* Manager::createAnimationDeath(CharacterOnBoard* animatedObj)
{
  return new Animations::Death(animatedObj);
}

Animation* Manager::createAnimationDisappear(CharacterOnBoard* animatedObj)
{
  return new Animations::Disappear(animatedObj, animatedObj->getActivity(), animatedObj->getDirection());
}

void Manager::addAfterThatAnimation(Animation* animation)
{
  // TODO przeszukać jeszcze raz wszystkie animacje i wstawić za nią
}

Sequence& Manager::addNewSetParallel()
{
  Sequence new_sequence;
  parallel_played_sequences.push_back(new_sequence);
  return parallel_played_sequences.back();
}

void Manager::addInNewSetAndBlock(Animation* animation)
{
  // tworzy nowy zbiór i zwraca
  Sequence& sequence = addNewSetParallel();

  // wrzucenie tam animacji
  sequence.push_back(animation);
}

int Manager::calculateTimeLeft_ms(Sequence& sequence, CharacterOnBoard* character)
{
  int time_in_ms = 0;
  int cumulated_time = 0;

  for (auto & animation : sequence)
  {
    cumulated_time += animation->getTimeLeft().asMilliseconds();

    if (character == nullptr)
    {
      time_in_ms = cumulated_time;
    }
    else
    {
      // jeżeli jest animacja na tej postaci, to musi dodać czas wykonania wszystkich przed nią
      if (animation->animatedObj == character)
      {
        time_in_ms = cumulated_time;
      }
    }
  }

  auto seq_left_time = sequence.timeToStart.asMilliseconds() - sequence.stoper.getElapsedTime().asMilliseconds();

  time_in_ms += seq_left_time;

  time_in_ms += 50; // TYMCZASOWE dodanie trochę dodatykowych ms odstępu żeby mieć pewność że animacja wykona się faktycznie po

  return time_in_ms;
}

int Manager::calculateTimeToLastAnimationFinish(CharacterOnBoard* character)
{
  int max_time_in_ms = 0;

  // znajdź czas po którym skończy się ostatnia ze wszystkich obecnych animacji ze wszystkich ścieżek
  for (auto sequence : parallel_played_sequences)
  {
    auto time_of_this_sequence = calculateTimeLeft_ms(sequence, character);

    if (max_time_in_ms < time_of_this_sequence)
      max_time_in_ms = time_of_this_sequence;
  }

  return max_time_in_ms;
}

Sequence& Manager::addNewSequenceParallelAfterTime(int time_in_ms)
{
  Sequence new_sequence;

  sf::Time maxTime = sf::milliseconds(time_in_ms);

  // pobiera czas do zakónczenia podanych sekwnencji
  new_sequence.timeToStart = maxTime;

  parallel_played_sequences.push_back(new_sequence);
  return parallel_played_sequences.back();
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

  // info, że animacja wykonała się do ostatniej klatki
  bool loop_reseted = false;

  // zwiększenie numeru klatki dla animowanego obiektu (dla klas animujących teksturę, tj. pochodnych OnTexture)
  if (dynamic_cast<OnTexture*>(&animation))
  {
    // animowanie sprita
    current_frame[object] += delta * sprites_data.fps;

    // utrzymanie wartości licznika animacji w jej zakresie
    while (current_frame[object] >= sprites_data.textures.size())
    {
      if (animation.finish_condition == Animations::FinishCondition::to_last_frame)
        // zakończenie animacji na ostatniej klatce
        current_frame[object] = sprites_data.textures.size() - 1;
      else
        // cofnięcie o całą rolkę do tyłu
        current_frame[object] -= sprites_data.textures.size();

      loop_reseted = true;
    }
    // ustawienie tekstury na zaokrąglony do int numer obecnej klatki tego obiektu
    object->setTexture(sprites_data.textures[static_cast<int>(current_frame[object])]);
  }

  // sprawdzanie czy spełniony został warunek kończący animację
  switch (animation.finish_condition)
  {
  case FinishCondition::no_repeat:
  {
    if (loop_reseted) {
      object->reset_texture();
      return true;
    }
  } break;
  case FinishCondition::to_last_frame:
  {
    if (loop_reseted) {
      return true;
    }
  } break;
  case FinishCondition::special_no_reset_texture:
  {
    if (animation.special_finish_condition_obtained())
    {
      return true;
    }
  } break;
  case FinishCondition::special:
  {
    if (animation.special_finish_condition_obtained())
    {
      object->reset_texture();
      return true;
    }
  } break;
  }

  // animacja nie zakończyła się
  return false;
}

void Manager::updateAnimationsStack(float delta)
{
  // lista animacji, która właśnie się zakończyła i będzie musiała zostać zdjęta
  std::vector<Animation*> animationsToErase;

  for (auto & sequence : parallel_played_sequences)
  {
    // poczekanie, aż upłynie wystarczająco dużo czasu do startu sekwencji
    if (sequence.stoper.getElapsedTime().asMilliseconds() < sequence.timeToStart.asMilliseconds())
      continue;

    if (sequence.empty())
      continue;

    // pobranie pierwszej animacji z każdej równolegle wykonywanej ścieżki
    auto animation_to_play = sequence.front();

    // jeżeli animacja się nie rozpoczęła, to jest teraz odpalana
    if (!animation_to_play->started)
    {
      try {
        animation_to_play->init();
      }
      catch (errors::no_anim_for_activity) {
        // w przeciwnym razie program zawiesiłby się
        std::cout << "Error! Animation for runned action does't exist.\n";
        std::exit(1);
      }

      // ustawienie animacji na początkową klatkę (tylko dla animacji z ustawionym isResettingFrame)
      if (animation_to_play->isResettingFrame) {
        current_frame[animation_to_play->animatedObj] = 0;
      }
    }

    // animowanie, a jeżeli animacja się zakończyła to będzię zdjęta
    if (update_animation_and_check_if_finished(*animation_to_play, delta)) {
      animationsToErase.push_back(animation_to_play);
    }
  }

  // usunięcie zakończonych animacji ze zbioru (i z pamięci)
  for (auto & an : animationsToErase)
  {
    bool quit_loop = false;

    for (auto & sequence : parallel_played_sequences)
    {
      if (sequence.empty())
        continue;

      for (std::vector<Animation*>::iterator it = sequence.begin(); it != sequence.end(); it++)
      {
        if (*it == an) {
          sequence.erase(it);
          delete an;
          quit_loop = true;
          break;
        }
      }
      if (quit_loop)
        break;
    }
  }

}

void Manager::updateIdleAnimations(std::vector<CharacterOnBoard*> characters,
                                   float delta)
{
  // zmiana statusu na idle wszystkim postaciom, dla których nie odgrywa się żadna animacja w tej chwili
  for (auto & character : characters)
  {
    bool any_animation_on_it = false;
    for (auto & sequence : parallel_played_sequences)
    {
      if (sequence.empty())
        continue;

      for (auto & animation : sequence)
      {
        // sprawdzenie czy jest obecnie jakaś animacja na tej postaci
        if (animation->animatedObj == character)
        {
          any_animation_on_it = true;
          break;
        }
      }
    }

    if (!any_animation_on_it)
      character->setActivity(Character::Activity::idle);
  }

  // update wszystkich animacji bezczynności
  for (auto & ani : idle_animations)
  {
    if (ani->animatedObj->getActivity() == Character::Activity::idle)
      update_animation_and_check_if_finished(*ani, delta);
  }
}

bool Manager::anyAnimationLocking()
{
  // sprawdzenie czy jakaś z oczekujących animacji blokuje
  for (auto & set : parallel_played_sequences) {
    for (auto & anim : set) {
      if (anim->isBlocking) {
        return true;
      }
    }
  }

  return false;
}
