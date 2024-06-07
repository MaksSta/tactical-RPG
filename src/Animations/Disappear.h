#ifndef ANIMATIONS_DISAPPEAR_H_
#define ANIMATIONS_DISAPPEAR_H_

#include "Animation.h"

namespace Animations 
{
    class Disappear : public Animation {
    public:
        /**
         * podstawowa animacja spritów
         * @param _animatedObj wskaźnik do postaci na której wykonywana będzie animacja
         * @param _activity podana aktywność, na podstawie której wybrany będzie odpowiedni zestaw spritów
         * @param _direction podany kierunek, na podstawie którego wybrany będzie odpowiedni zestaw spritów
        */ 
        Disappear(  CharacterOnBoard* _animatedObj,
                    Character::Activity _activity,
                    Direction _direction);

        // inicjalizacja animacji, w praktyce zmienia tylko stan postaci
        void init();

        // zwiększa poziom przezroczystości
        virtual void animate(float delta);

        // warunek końcowy animacji - przezroczystość osiągnęła 255
        bool special_finish_condition_obtained() const;
    protected:
        unsigned char transparency {0};

        // aktywność jaką otrzyna postać przy wywołaniu animacji
        Character::Activity activity;

        // kierunek jaki otrzyma postać przy wywołaniu animacji
        Direction direction;
    }; 
}

#endif /* ANIMATIONS_DISAPPEAR_H_ */