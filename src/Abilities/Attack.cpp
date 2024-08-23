#include "Attack.h"

Attack::Attack(Character::Activity _activity,
               CallType _callType,
               int _action_points,
               int range_min,
               int range_max,
               int _min_dmg,
               int _max_dmg,
               bool radiation)
  :
  activity{_activity},
  callType{_callType},
  action_points{_action_points},
  min_dmg{_min_dmg},
  max_dmg{_max_dmg}
{
  if (range_max == 1)
    type = Type::melee;
  else if (range_max > 1)
    type = Type::ranged;

  for (int x = -range_max; x <= range_max; x++)
    for (int y = -range_max; y <= range_max; y++) {
      // pominięcie pola na którym jest atakujący
      if (x == 0 && y == 0)
        continue;

      // pominięcie pól z mniejszym zasięgiem niż zasięg minimalny
      if (abs(x) + abs(y) < range_min)
        continue;

      // pominięcie wszystkich pól innych niż poziomej i pionowej linii od atakującego
      if (!radiation && (x && y))
        continue;

      // dla ustawionego radiation, pomija pola których suma przesunięć od postaci jest większa niż zasięg ataku
      if (radiation && (abs(x) + abs(y) > range_max))
        continue;

      in_range.push_back({x,y});
    }
}

Attack::Type Attack::get_type()
{
  return type;
}

int Attack::get_min_dmg()
{
  return min_dmg;
}

int Attack::get_max_dmg()
{
  return max_dmg;
}

int Attack::get_dmg_variety()
{
  return max_dmg - min_dmg;
}

int Attack::draw_damage()
{
  return min_dmg + (get_dmg_variety() ? std::rand() % get_dmg_variety() : 0);
}

int Attack::getAP()
{
  return action_points;
}

Character::Activity Attack::getActivity()
{
  return activity;
}

Attack::CallType Attack::getCallType()
{
  return callType;
}
