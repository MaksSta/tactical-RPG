#ifndef AI_ACTIONS_MOVE_H_
#define AI_ACTIONS_MOVE_H_

#include "Action.h"

namespace AI
{
  class Move : public Action {
  public:
    Move(Road road);

    Action* clone() const override;

    Road& getRoad() override;
  private:
    Road road;
  };
}

#endif /* AI_ACTIONS_MOVE_H_ */
