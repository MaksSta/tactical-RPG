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

  Type get_type() const;

  int get_min_dmg() const;
  int get_max_dmg() const;
  int get_dmg_variety() const;

  // wylosowanie obrażeń z przedziału [min_dmg, max_dmg]
  int draw_damage() const;

  int getAP() const;

  Character::Activity getActivity() const;
  CallType getCallType() const;
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
