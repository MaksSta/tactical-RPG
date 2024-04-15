#include "Attack.h"

Attack::Attack(int _action_points, int range, int _min_dmg, int _max_dmg)
:
    action_points{_action_points},
    min_dmg{_min_dmg},
    max_dmg{_max_dmg}
{
    if (range == 1)
        type = Type::melee;
    else if (range > 1)
        type = Type::ranged;

    for (int x = -range; x <= range; x++)
        for (int y = -range; y <= range; y++)
        {
            // pominięcie pola na którym jest atakujący oraz pól po skosie
            if (x == 0 && y == 0 || x && y)
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