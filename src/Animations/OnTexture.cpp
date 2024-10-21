#include "OnTexture.h"

using namespace Animations;

OnTexture::OnTexture(CharacterOnBoard* _animatedObj,
                     Character::Activity _activity,
                     Direction _direction,
                     FinishCondition _finish_condition)
{
  animatedObj = _animatedObj;
  activity = _activity;
  direction = _direction;
  finish_condition = _finish_condition;
}

void OnTexture::init()
{
  // zmiana stanu postaci
  animatedObj->setActivity(activity);
  animatedObj->setDirection(direction);

  // sprawdzenie czy istnieje animacja dla tej aktywności postaci
  if(animatedObj->get_current_sprites_data().textures.empty())
    throw errors::no_anim_for_activity();
}

//void OnTexture::animate(float delta) {}
