#include "Character.h"

Character::Character(std::string _name, int _max_hp)
:
    name{_name},
	max_hp{_max_hp},
	hp{_max_hp}
{
	// ustawienie punktów akcji na domyślną wartość
	max_action_points = 6;
	action_points = max_action_points;
}

Character::Activity Character::get_activity()
{
	return currentActivity;
}

void Character::setActivity(Character::Activity _activity)
{
	currentActivity = _activity;
}

Direction Character::getDirection() 
{
	return direction;
}

void Character::setDirection(Direction _direction)
{
	direction = _direction;
}

int Character::getAP()
{
	return action_points;
}

void Character::setAP(int ap)
{
	action_points = ap;
}

int Character::getHP()
{
	return hp;
}

void Character::setHP(int _hp)
{
	hp = _hp;
}

std::string Character::getName()
{
	return name;
}