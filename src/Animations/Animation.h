/**
 * Interfejs dla animacji
 * wskazuje obiekt dla którego wywoływana jest animacja
 */

#ifndef ANIMATIONS_ANIMATION_H_
#define ANIMATIONS_ANIMATION_H_

#include "../Characters/CharacterOnBoard.h"

namespace Animations
{
  // błędy jakie można napotkać podczas animacji
  namespace errors
  {
    struct no_anim_for_activity{};
    struct undefined_special_condition{};
  }
  enum FinishCondition {
    no_repeat,
    to_last_frame,
    special,
    special_no_reset_texture
  };

  class Animation {
  public:
    // wskaźnik do postaci na której wykonywana będzie animacja
    CharacterOnBoard* animatedObj {nullptr};

    // funkcja wywoływana co klatkę animacji, możliwy override
    virtual void animate(float delta) {}

    virtual void init() = 0;

    virtual bool special_finish_condition_obtained() const {
      // specjalny warunek musi zostać napisany jako override, tam gdzie chcemy wpisać własny warunek zakończenia
      // (domyślnie animacja kończy się w zależności od finishCondition)
      throw errors::undefined_special_condition();
    }

    // warunek końcowy zakóńczenia animacji
    FinishCondition finish_condition;

    // czy ta animacja przy inicjowaniu przestawia numer klatkę na początkową
    bool isResettingFrame{true};

    // czy podczas wywoływania tej animacji decyzje w grze pozostają zablokowane aż do jej zakończenia
    bool isBlocking{true};
  };


}

#endif /* ANIMATIONS_ANIMATION_H_ */
