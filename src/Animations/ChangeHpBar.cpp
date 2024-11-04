#include "ChangeHpBar.h"

using namespace Animations;

ChangeHpBar::ChangeHpBar(CharacterOnBoard *_animatedObj,
                         int _dmg)
  :
  dmg{_dmg},
  hpBar{_animatedObj->getHpBar()}
{
  animatedObj = _animatedObj;
  isBlocking = false;
  finish_condition = FinishCondition::special_no_reset_texture;

  duration = sf::milliseconds(300);
}

void ChangeHpBar::init() {
  Animation::init();
  hpBar.takeDamage(dmg);
}

bool ChangeHpBar::special_finish_condition_obtained() const
{
  // na pasku życia wykonała się już animacja
  return !(hpBar.isTakingDamage());
}
