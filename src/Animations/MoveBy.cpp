#include "MoveBy.h"

using namespace Animations;

MoveBy::MoveBy( CharacterOnBoard* _animatedObj,
                Actions::Move& move,
                bool _isBlocking)
  :
  OnTexture(_animatedObj, Character::move, move.direction, FinishCondition::special),
  offset(move.offset),
  speed(move.speed)
{
  isResettingFrame = false;
  isBlocking = _isBlocking;
}

void MoveBy::init()
{
  auto startPos = animatedObj->getPosition();
  dstPos = startPos + offset;

  OnTexture::init();
}

void MoveBy::animate(float delta)
{
  // wyliczenie offsetu jako różnicy punktu docelowego a obecnego położenia
  sf::Vector2f offset;
  offset = dstPos - animatedObj->getPosition();

  float dx = std::min(delta * speed, static_cast<float>(abs(offset.x))) * (offset.x > 0 ? 1 : -1);
  float dy = std::min(delta * speed, static_cast<float>(abs(offset.y))) * (offset.y > 0 ? 1 : -1);

  // zapisanie pozycji jeszcze przed przesunięciem w obecnej klatce (b4 - before)
  sf::Vector2f pos_b4 = animatedObj->getPosition();

  // wykonanie przesunięcia
  animatedObj->move({dx, dy});

  // postać dotarła na miejsce docelowe
  if (animatedObj->getPosition() == pos_b4) {
    // zabezpieczenie dla przypadku gdy zostało przesunięcie za małe, by move coś ruszyło
    animatedObj->setPosition(dstPos);
  }
}

bool MoveBy::special_finish_condition_obtained() const
{
  // postać stoi na docelowej pozycji
  if (animatedObj->getPosition() == dstPos)
    return true;

  return false;
}
