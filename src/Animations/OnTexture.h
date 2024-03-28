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
        OnTexture(  CharacterOnBoard* _animatedObj,
                    Character::Activity _activity,
                    Direction _direction,
                    FinishCondition _finish_condition = no_repeat );

        // inicjalizacja animacji, w praktyce zmienia tylko stan postaci
        void init();

        // w przypadku OnTexture metoda ta nie wykonuje żadnych działań, gdyż klatkami zarządza Manager
        // w celu wywoływania dodatkowych działań w czasie animacji należy zastąpić tę metodę w klasie pochodnej
        virtual void animate(float delta);

        // specjalny warunek animacji - metoda to musi zostać odziedziczona
        bool special_finish_condition_obtained() const;
    protected:
        // aktywność jaką otrzyna postać przy wywołaniu animacji
        Character::Activity activity;

        // kierunek jaki otrzyma postać przy wywołaniu animacji
        Direction direction;
    };

    // błędy jakie można napotkać podczas animacji
    namespace errors 
    {
        struct no_anim_for_activity {};
        struct undefined_special_condition {};
    }
    
}

#endif /* ANIMATIONS_ONTEXTURE_H_ */