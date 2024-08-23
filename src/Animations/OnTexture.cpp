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

void OnTexture::animate(float delta) {}

bool OnTexture::special_finish_condition_obtained() const {
  // zgłoszenie wyjątku, ponieważ specjalny warunek występuje tylko w wyspecjalizowancyh klasach pochodnych
  // skoro nie został odziedziczony oznacza to, że klasa dla której wywołano tę funkcję ma inny waurnek końcowy
  // jeżeli zgłoszono, sprawdzić czy finish_condition nie został niesłusznie ustawiony na special
  // skoro ma zostać sprawdzony własny niestandardowy warunek zakończenia animacji, należy zdefiniować tę metodę w klasie pochodnej
  throw errors::undefined_special_condition();
}
