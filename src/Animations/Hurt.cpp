#include "Hurt.h"

using namespace Animations;

Hurt::Hurt( CharacterOnBoard* _animatedObj,
            int _damage)
:
	OnTexture(_animatedObj, Character::hurt, _animatedObj->getDirection()),
    damage{_damage}
{
}

void Hurt::init()
{
    OnTexture::init();

    // otrzymanie obrażeń przez animowaną postać
    animatedObj->takeDamage(damage);
}
