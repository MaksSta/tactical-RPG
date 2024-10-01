#include "Attack.h"

using namespace AI;

Attack::Attack(AttackInfo& _attackInfo)
  :
  attackInfo{_attackInfo}
{}

Action* Attack::clone() const
{
	return new Attack(*this);
}

AttackInfo& Attack::getAttackInfo()
{
  return attackInfo;
}