/**
 * Klasa bazowa do wszystkich animacji, gdzie animowane są sprity
 * może służyć jako baza do bardziej rozbudowanych animacji
 */

#ifndef ANIMATIONS_ONTEXTURE_H_
#define ANIMATIONS_ONTEXTURE_H_

#include "Animation.h"

namespace Animations
{
  class OnTexture : public Animation {
  public:
    /**
     * podstawowa animacja spritów
     * @param _animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param _activity podana aktywność, na podstawie której wybrany będzie odpowiedni zestaw spritów
     * @param _direction podany kierunek, na podstawie którego wybrany będzie odpowiedni zestaw spritów
     * @param _finish_condition rodzaj warunku jaki zakończy animację
     */
    OnTexture(CharacterOnBoard* _animatedObj,
              Character::Activity _activity,
              Direction _direction,
              FinishCondition _finish_condition = no_repeat);

    // inicjalizacja animacji, w praktyce zmienia tylko stan postaci
    virtual void init();
  protected:
    // aktywność jaką otrzyna postać przy wywołaniu animacji
    Character::Activity activity;

    // kierunek jaki otrzyma postać przy wywołaniu animacji
    Direction direction;
  };
}

#endif /* ANIMATIONS_ONTEXTURE_H_ */
