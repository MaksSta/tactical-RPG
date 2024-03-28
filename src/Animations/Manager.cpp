#include "Manager.h"

using namespace Animations;

Manager::Manager() {}

Animation* Manager::createAnimation(	CharacterOnBoard* animatedObj,
										Character::Activity activity,
										Direction direction)
{
	return new Animations::OnTexture(animatedObj, activity , direction);
}

Animation* Manager::createAnimation(	CharacterOnBoard* animatedObj,
										Character::Activity activity)
{
	auto direction = animatedObj->getDirection();

	return new Animations::OnTexture(animatedObj, activity , direction);
}

Animation* Manager::createAnimationMove(	CharacterOnBoard* animatedObj,
											Animations::Actions::Move move)
{
	return new Animations::MoveBy(animatedObj, move);
}

Animation* Manager::createAnimationHurt(	CharacterOnBoard* animatedObj,
											int damage)
{
	return new Animations::Hurt(animatedObj, damage);
}


void Manager::addAnimationToQueue(Animation* animation)
{
	main_queue.push(animation);
}

void Manager::addIdleAnimation(Animation* animation)
{
	idle_animations.push_back(animation);
}

bool Manager::update_animation_and_check_if_finished(	Animation& animation,
													 	float delta)
{
	animation.animate(delta);

	auto object = animation.animatedObj;
	auto & sprites_data = object->get_current_sprites_data();

	// zwiększenie numeru klatki dla animowanego obiektu
	current_frame[object] += delta * sprites_data.fps;

	// do zanotowania jeżeli animacja wróciła do początkowej klatki
	bool loop_reseted = false;	

	// utrzymanie wartości licznika animacji w jej zakresie
	while (current_frame[object] >= sprites_data.textures.size() ) {
		current_frame[object] -= sprites_data.textures.size();
		loop_reseted = true;
	}

	// ustawienie tekstury na zaokrąglony do int numer obecnej klatki tego obiektu
	object->setTexture(
		sprites_data.textures[static_cast<int>(current_frame[object])]
	);

	// sprawdzanie czy spełniony został warunek kończący animację
	switch (animation.finish_condition) {
		case no_repeat:
		{
			if (loop_reseted)
				return true;
		}
		break;
		case special:
		{
			if (animation.special_finish_condition_obtained())
				return true;
		}
		break;
	}
	return false;
}

void Manager::updateAnimationsStack(float delta)
{
	// wskaźnik na ostatnio odtwarzaną animację 
	// do porówywania czy animacja zmieniła się od ostatniego wywołania funkcji
	Animation* previousAnimation;

	// w przypadku braku jakichkolwiek animacji w kolejce nie wykonuje nic
	if (main_queue.empty())
		return;

	// w przypadku braku aktywnej animacji wrzucenie pierwszej ze stosu (o ile jakaś została)
	if (!currentAnimation) {
		if (!main_queue.empty()) {
			currentAnimation = main_queue.front();
			// przy opdaleniu nowej animacji jest ona wpierw inicjowana
			if (currentAnimation != previousAnimation) {
				// wychwycenie wyjątku w przypadku próby uruchomienia animacji która nie istnieje
				try {
					currentAnimation->init();
				}
				catch (errors::no_anim_for_activity) {
					// w przeciwnym razie program zawiesiłby się
					std::cout << "Error! Animation for runned action does't exist.\n";
					std::exit(1);
				}
				// ustawienie animacji na początkową klatkę jeżeli taką informację animacja
				if (currentAnimation->isResettingFrame) {
					current_frame[currentAnimation->animatedObj] = 0;
				}
			}
		} 
	}

	if (currentAnimation) {
		// update bieżącej animacji
		// w przypadku spełnienia warunku końcowego zakończonenie animacji
		if (update_animation_and_check_if_finished(*currentAnimation, delta)) {
			
			// reset stanu aktywności postaci 
			currentAnimation->destroy();

			// powrót do domyślnego spritu
			currentAnimation->animatedObj->reset_texture();

			delete currentAnimation;
			
			currentAnimation = nullptr;
			main_queue.pop();
		}
	}
	previousAnimation = currentAnimation;
}

void Manager::updateIdleAnimations(float delta) 
{
	// update wszystkich animacji bezczynności
	for( auto & ani : idle_animations) {
		if (ani->animatedObj->get_activity() == Character::Activity::idle)
			update_animation_and_check_if_finished(*ani, delta);
	}
}

bool Manager::anyAnimationLocking()
{
	if (main_queue.empty())
		return false;
    return true;
}
