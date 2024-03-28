/** 
 * Klasa zarządzająca animacjami
 * Odpowiada za:
 * - obługę równolegle wykonywanych animacji 
 * - obsługę kolejki animacji wykonanych jedna po drugiej:
 *   - inicjalizację nowo zdjętej animacji z kolejki (co dodatkowo może wywoływać różne zdarzenia)
 *   - animowanie jej, aż do spełenienia jej warunku końcowego
 *   - usunięcie po jego spełnieniu
*/

#ifndef ANIMATIONS_MANAGER_H_
#define ANIMATIONS_MANAGER_H_

#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>
#include <map>
#include <vector>

#include "../Characters/Character.h"

#include "MoveBy.h"
#include "Hurt.h"

#include "Actions/Move.h"

namespace Animations 
{
    class Manager {
    public:
        // jedyny i domyślny konstruktor, nie robi zupełnie nic
        Manager();

        /**
         * tworzy dowolną animację
         * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
         * @param activity podana aktywność, na podstawie której wybrany będzie odpowiedni zestaw spritów
         * @param direction podany kierunek, na podstawie którego wybrany będzie odpowiedni zestaw spritów
         * @return zwraca wskaźnik do nowo utworzonej animacji
        */ 
        Animation* createAnimation( CharacterOnBoard* animatedObj,
                                    Character::Activity activity,
                                    Direction direction);

        /**
         * tworzy dowolną animację o kierunku w jakim obrócona jest obecnie postać
         * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
         * @param activity podana aktywność, na podstawie której wybrany będzie odpowiedni zestaw spritów
        */ 
        Animation* createAnimation( CharacterOnBoard* animatedObj,
                                    Character::Activity activity);

        /** 
         * tworzy animację z jednoczesnym przesunięciem na ekranie
         * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
         * @param move to akcja ruchu ze specjalnej grupy klas do akcji w animacjach
         * @return zwraca wskaźnik do nowo utworzonej animacji
        */ 
        Animation* createAnimationMove( CharacterOnBoard* animatedObj,
                                        Animations::Actions::Move move);

        /** 
         * tworzy animcję otrzymywania obrażeń, która redukuje punkty życia postaci
         * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
         * @param damage to ilość punktów obrażeń, jaką otrzyma postać podczas animacji
         * @return zwraca wskaźnik do nowo utworzonej animacji
        */ 
        Animation* createAnimationHurt( CharacterOnBoard* animatedObj, int damage);

        // dodanie animacji do kolejki
        void addAnimationToQueue(Animation* animation);

        // dodanie animacji do wektora równolegle wywoływanych animacji bezczynności
        void addIdleAnimation(Animation* animation);

        // wykonanie animacji dla bieżącej klatki
        void updateAnimationsStack(float delta);

        // update wszystkich animacji bezczynności
        void updateIdleAnimations(float delta);

        // update wybranej animacj i zwrócenie wyniku o tym czy się zakończyła
        bool update_animation_and_check_if_finished(Animation& animation, float delta);

        // zwraca informację czy zostały jakieś animacje w kolejce (blokują one tryb gry)
        bool anyAnimationLocking();
    private:
        // główna ścieżka animacji wykonywanych kolejno po sobie (blokują tryb gry)
        std::queue<Animation*> main_queue;

        // wskaźnik do obecnie obsługiwanej animacji z kolejki
        Animation* currentAnimation {nullptr};

        // równolegle wykonywane animacje bezczynności (nie blokują trybu gry)
        std::vector<Animation*> idle_animations;

        // klatka na której jest animacja dla wybranej postaci
        std::map<CharacterOnBoard*, float> current_frame;
    };

}
#endif /* ANIMATIONS_MANAGER_H_ */