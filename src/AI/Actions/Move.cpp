#include "Move.h"

using namespace AI;

Move::Move(Road _road)
  :
  road{_road}
{}

Action* Move::clone() const
{
	return new Move(*this);
}

Road& Move::getRoad()
{
  return road;
}