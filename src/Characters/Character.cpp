#include "Character.h"

Character::Character(std::string _name, Team _team, int _max_hp)
  :
  name{_name},
  team{_team},
  max_hp{_max_hp},
  hp{_max_hp}
{
  // ustawienie punktów akcji na domyślną wartość
  max_action_points = 6;
  action_points = max_action_points;
}

void Character::die()
{
  setActivity(Character::Activity::death);
  alive = false;
}

bool Character::isAlive()
{
  return alive;
}

Character::Team Character::getTeam()
{
  return team;
}

Character::Activity Character::getActivity()
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

int Character::getMaxAP()
{
  return max_action_points;
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
