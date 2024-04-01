/**
 * Klasa zawierająca wszystkie potrzebne informacje o ataku jaki może wywołać postać
*/

#ifndef ABILITIES_ATTACK_H_
#define ABILITIES_ATTACK_H_

#include "Ability.h"

class Attack : public Ability {
public:
    // rodzaje ataków
    enum Type {
        melee,
        ranged
    };

    Attack(int range, int _min_dmg, int _max_dmg);

    // zwraca rodzaj ataku
    Type get_type();

    int get_min_dmg();
    int get_max_dmg();
    int get_dmg_variety();

    // wylosowanie obrażeń z przedziału [min_dmg, max_dmg]
    int draw_damage();
private:
    // rodzaj ataku
    Type type;

    int min_dmg;
    int max_dmg;
};

#endif /* ABILITIES_ATTACK_H_ */