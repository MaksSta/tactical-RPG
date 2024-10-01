#ifndef AI_ACTIONS_FINISHTURN_H_
#define AI_ACTIONS_FINISHTURN_H_

#include "Action.h"

namespace AI
{
  class FinishTurn : public Action {
  public:
    Action* clone() const override;
  };
}

#endif /* AI_ACTIONS_FINISHTURN_H_ */
