/**
 * Wszystkie potrzebne informacje o ataku jaki może wywołać postać
 */

#ifndef ABILITIES_ATTACK_H_
#define ABILITIES_ATTACK_H_

#include "Ability.h"

class Attack : public Ability {
public:
  enum Type {
    melee,
    ranged
  };

  enum CallType {
    targetable,
    instant
  };

  Attack(Character::Activity _activity,
         CallType _callType,
         int _action_points,
         int range_min,
         int range_max,
         int _min_dmg,
         int _max_dmg,
         bool radiation = false);

  Type get_type();

  int get_min_dmg();
  int get_max_dmg();
  int get_dmg_variety();

  // wylosowanie obrażeń z przedziału [min_dmg, max_dmg]
  int draw_damage();

  int getAP();

  Character::Activity getActivity();
  CallType getCallType();
private:
  Character::Activity activity;
  CallType callType;

  // koszt wywołania ataku
  int action_points;

  Type type;

  int min_dmg;
  int max_dmg;
};

#endif /* ABILITIES_ATTACK_H_ */
