#include "Character.h"

Character::Character(std::string _name,
                     Team _team,
                     int _max_hp)
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

bool Character::isAlive() const
{
  return !will_die_this_turn;
}

Character::Team Character::getTeam() const
{
  return team;
}

Character::Activity Character::getActivity() const
{
  return currentActivity;
}

void Character::setActivity(Character::Activity _activity)
{
  currentActivity = _activity;
}

Direction Character::getDirection() const
{
  return direction;
}

void Character::setDirection(Direction _direction)
{
  direction = _direction;
}

int Character::getAP() const
{
  return action_points;
}

void Character::setAP(int ap)
{
  action_points = ap;
}

int Character::getMaxAP() const
{
  return max_action_points;
}

int Character::getHP() const
{
  return hp;
}

void Character::setHP(int _hp)
{
  hp = _hp;
}

std::string Character::getName() const
{
  return name;
}
