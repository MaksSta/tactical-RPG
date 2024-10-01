#ifndef AI_ACTIONS_ACTION_H_
#define AI_ACTIONS_ACTION_H_

#include "../../GameEngine/Road.h"
#include "../../GameEngine/Range.h"
#include "../../Abilities/Attack.h"

class CharacterOnBoard;

struct AttackInfo {
  Abilities::Attack* attack;
  CharacterOnBoard* target;
  Range range;
};

namespace errors {
  struct invalid_usage_of_action{};
}

namespace AI
{
  class Action {
  public:
    virtual Action* clone() const = 0;
    virtual Road& getRoad();
    virtual AttackInfo& getAttackInfo();
  };
}

#endif /* AI_ACTIONS_ACTION_H_ */
