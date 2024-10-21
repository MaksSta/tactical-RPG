#include "Hurt.h"

using namespace Animations;

Hurt::Hurt(CharacterOnBoard* _animatedObj)
  :
  OnTexture(_animatedObj, Character::hurt, _animatedObj->getDirection())
{
  isBlocking = false;
}
