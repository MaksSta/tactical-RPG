/**
 * Animacja zdejmowania hp z pasku życia
 */

#ifndef ANIMATIONS_CHANGEHPBAR_H_
#define ANIMATIONS_CHANGEHPBAR_H_

#include "../Characters/HpBar.h"
#include "Animation.h"

namespace Animations
{
  class ChangeHpBar : public Animation {
  public:
    /**
    * animacja ta redukuje punkty życia na pasku
    * @param _animatedObj wskaźnik do postaci na której wykonywana będzie
    * animacja
    * @param _dmg ilość otrzymanych obrażeń
    */
    ChangeHpBar(CharacterOnBoard *_animatedObject, int _dmg);

    void init();

    bool special_finish_condition_obtained() const;
  private:
    int dmg;
    HpBar& hpBar;
  };
} // namespace Animations

#endif /* ANIMATIONS_CHANGEHPBAR_H_ */
