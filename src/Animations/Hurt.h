/**
 * Animacja podczas której postać otrzymuje obrażenia
 * jednocześnie powstaje efekt zmniejszania punktów życia na pasku w czasie
 */

#ifndef ANIMATIONS_HURT_H_
#define ANIMATIONS_HURT_H_

#include "OnTexture.h"

namespace Animations
{
  class Hurt : public OnTexture {
  public:
    /**
     * animacja otrzymywania obrażeń, przy wywołaniu redukuje punkty życia postaci
     * @param _animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     */
    Hurt(CharacterOnBoard* _animatedObject);
  };
}

#endif /* ANIMATIONS_HURT_H_ */
