#include "Action.h"

using namespace AI;

AttackInfo& Action::getAttackInfo()
{
  throw errors::invalid_usage_of_action();
}

Road& Action::getRoad()
{
  throw errors::invalid_usage_of_action();
}