#ifndef AI_ACTIONS_ATTACK_H_
#define AI_ACTIONS_ATTACK_H_

#include "Action.h"

namespace AI
{
  class Attack : public Action {
  public:
    Attack(AttackInfo& _attackInfo);

    Action* clone() const override;

    AttackInfo& getAttackInfo() override;
  private:
    AttackInfo attackInfo;
  };
}

#endif /* AI_ACTIONS_ATTACK_H_ */
