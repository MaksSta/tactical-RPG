#include "FinishTurn.h"

using namespace AI;

Action* FinishTurn::clone() const
{
  return new FinishTurn(*this);
}