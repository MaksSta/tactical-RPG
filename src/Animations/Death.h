/**
 * Animacja podczas której postać umiera
 */

#ifndef ANIMATIONS_DEATH_H_
#define ANIMATIONS_DEATH_H_

#include "OnTexture.h"

namespace Animations
{
  class Death : public OnTexture {
  public:
    /**
     * animacja umierania, która przestawia status postaci na umierający
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     */
    Death(CharacterOnBoard* _animatedObject);

    // zadanie animowanej postaci obrażeń i wywołanie init z klasy bazowej
    void init();
  };
}

#endif /* ANIMATIONS_DEATH_H_ */
