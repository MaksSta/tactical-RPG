/**
 * Animacja podczas której postać otrzymuje obrażenia
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
         * @param _damage to ilość punktów obrażeń, jaką otrzyma postać podczas animacji
        */ 
        Hurt(CharacterOnBoard* _animatedObject, int _damage);

        // zadanie animowanej postaci obrażeń i wywołanie init z klasy bazowej
        void init();
    protected:
        // obrażenia do zadania
        int damage;
    };

}

#endif /* ANIMATIONS_HURT_H_ */