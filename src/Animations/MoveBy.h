/**
 * Animuje postać i przesuwa jej pozycję na ekranie
 */

#ifndef ANIMATIONS_MOVEBY_H_
#define ANIMATIONS_MOVEBY_H_

#include <cmath>

#include "OnTexture.h"
#include "Actions/Move.h"

namespace Animations
{
  class MoveBy : public OnTexture {
  public:
    /**
     * animacja przesunięcia wraz z animacją sprita
     * @param _animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param move to akcja ruchu ze specjalnej grupy klas do akcji w animacjach
     */
    MoveBy(CharacterOnBoard* _animatedObject,
           Actions::Move& move);

    // wyzaczenie punktu końcowego i wywołanie init z klasy bazowej
    void init();
  protected:
    // wykonywanie przesunięcia na ekranie
    virtual void animate(float delta);

    // przesunięcie jakie zostało do wykonania
    sf::Vector2f offset;

    // prędkość poruszania w pikselach/sekundę
    float speed;

    // warunek końcowy - animowany obiekt dotarł na docelową pozycję
    bool special_finish_condition_obtained() const;

    // pozycja docelowa
    sf::Vector2f dstPos;
  };
}

#endif /* ANIMATIONS_MOVEBY_H_ */
