#include "Move.h"

using namespace Animations::Actions;

Move::Move(sf::Vector2f _offset,
           float _speed)
  :
  offset{_offset},
  speed{_speed}
{
  /**
   * @NIEZMIENNIK:
   * Wartość prędkości przesunięcia nie może być zerowa
   */
  if (speed == 0)
    throw errors::zero_speed();

  /**
   * @NIEZMIENNIK:
   * Musi istnieć tylko jedna niezerowa współrzędna przesunięcia
   * (Poruszanie po skosie jest zabronione)
   */
  if (offset.x && offset.y)
    throw errors::diagonal_move();

  /**
   * @NIEZMIENNIK:
   * Obie współrzędne naraz nie mogą mieć zerowej wartości
   */
  if (!offset.x && !offset.y)
    throw errors::zero_move();

  // wyznaczenie kierunku ruchu na podstawie wartości offsetu
  if (offset.x > 0) {
    direction = right;
  }
  else if (offset.x < 0) {
    direction = left;
  }
  else if (offset.y > 0) {
    direction = down;
  }
  else if (offset.y < 0) {
    direction = up;
  }
}
