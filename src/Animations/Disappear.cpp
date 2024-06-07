#include "Disappear.h"

using namespace Animations;

Disappear::Disappear(   CharacterOnBoard* _animatedObj,
                        Character::Activity _activity,
                        Direction _direction)
{
    animatedObj = _animatedObj;
	activity = _activity;
	direction = _direction;

    finish_condition = special;
    isResettingFrame = false;
}

void Disappear::animate(float delta)
{
    transparency += delta * 225.f;
    animatedObj->setTransparency(255 - transparency);
}

void Disappear::init()
{
    animatedObj->disableHpBar();
}

bool Disappear::special_finish_condition_obtained() const
{
    if (transparency >= 255)
        return true;
    return false;
}