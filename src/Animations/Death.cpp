#include "Death.h"

using namespace Animations;

Death::Death( CharacterOnBoard* _animatedObj)
:
	OnTexture(_animatedObj, Character::death, _animatedObj->getDirection(), to_last_frame)
{
}

void Death::init()
{
    OnTexture::init();

    // śmierć postaci
    animatedObj->die();
}
