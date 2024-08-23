/**
 * Interfejs dla animacji
 * wskazuje obiekt dla którego wywoływana jest animacja
 */

#ifndef ANIMATIONS_ANIMATION_H_
#define ANIMATIONS_ANIMATION_H_

#include "../Characters/CharacterOnBoard.h"

namespace Animations
{
  enum FinishCondition {
    no_repeat,
    to_last_frame,
    special,
  };

  class Animation {
  public:
    // wskaźnik do postaci na której wykonywana będzie animacja
    CharacterOnBoard* animatedObj {nullptr};

    virtual void animate(float delta) = 0;
    virtual void init() = 0;
    virtual bool special_finish_condition_obtained() const = 0;

    // warunek końcowy zakóńczenia animacji
    FinishCondition finish_condition;

    // czy ta animacja przy inicjowaniu przestawia numer klatkę na początkową
    bool isResettingFrame{true};
  };
}

#endif /* ANIMATIONS_ANIMATION_H_ */
