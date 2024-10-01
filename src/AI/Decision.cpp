#include "Decision.h"

using namespace AI;

Decision::Decision(const Decision& org_decision)
{
  for (auto & a : org_decision.actions) {
    actions.push_back(a->clone());
  }
}

Decision& Decision::operator=(Decision const& org_decision)
{
  for (auto & a : org_decision.actions)
    actions.push_back(a->clone());

  return *this;
}

Decision::Decision()
{}

void Decision::pushAction(Move move)
{
  actions.push_back(new Move(move));
}

void Decision::pushAction(Attack attack)
{
  actions.push_back(new Attack(attack));
}

void Decision::pushAction(FinishTurn finishturn)
{
  actions.push_back(new FinishTurn(finishturn));
}

Action* Decision::popNextAction()
{
  auto action = actions.front();
  if(action) {
    actions.pop_front();
    return action;
  } else
    return nullptr;
}

bool Decision::empty() const
{
  return actions.empty();
}

Decision::~Decision()
{
  for (auto * a : actions)
    delete a;
}