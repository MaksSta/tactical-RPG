#ifndef AI_DECISION_H_
#define AI_DECISION_H_

#include "../GameEngine/ActiveBoard.h"
#include "Actions/FinishTurn.h"
#include "Actions/Move.h"
#include "Actions/Attack.h"

#include <deque>

namespace AI
{
  class Decision {
  public:
    Decision();
    Decision(const Decision& org_decision);
    Decision& operator=(Decision const& org_decision);

    void pushAction(Move move);
    void pushAction(Attack attack);
    void pushAction(FinishTurn finishTurn);

    Action* popNextAction();
    bool empty() const;
    
    ~Decision();
  private:
    std::deque<Action*> actions;
  };
}

#endif /* AI_DECISION_H_ */