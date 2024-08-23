/**
 * Klasa zarządzająca animacjami
 * Odpowiada za:
 * - wygenerowanie wszelkich animacji animacji na podstawie wytycznych
 * - obługę równolegle wykonywanych animacji bezczyności
 * - obsługę kolejki zbiorów animacji pobieranych jeden po drugim
 * - równlogłe wykonanie wszystkich animacji z obecnie wywoływanego zbioru:
 *   - inicjalizacja nowo zdjętej animacji z kolejki
 *     (co dodatkowo może wywoływać różne zdarzenia np. animacja Hurt zabiera punkty HP)
 *   - animowanie jej, aż do spełenienia jej warunku końcowego
 *   - usunięcie po jego spełnieniu
 *
 *  Graficzne przedstawienie przykładowego obsłużenia czterech zbiórów animacji ABCD
 *  ponumerowano wszystkie animacje w jednym zbiorze, znaki === oznaczają czas trwania animacji
 *
 *  [A1===][B1==]  [C1=====]   [D1==]
 *         [B2====][C2===]
 *                 [C3========]
 */

#ifndef ANIMATIONS_MANAGER_H_
#define ANIMATIONS_MANAGER_H_

#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include "../Characters/Character.h"

#include "MoveBy.h"
#include "Hurt.h"
#include "Death.h"
#include "Disappear.h"

#include "Actions/Move.h"

namespace Animations
{
  class Manager {
  public:
    /**
     * tworzy dowolną animację
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param activity podana aktywność, na podstawie której wybrany będzie odpowiedni zestaw spritów
     * @param direction podany kierunek, na podstawie którego wybrany będzie odpowiedni zestaw spritów
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimation(CharacterOnBoard* animatedObj,
                               Character::Activity activity,
                               Direction direction);

    /**
     * tworzy dowolną animację o kierunku w jakim obrócona jest obecnie postać
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param activity podana aktywność, na podstawie której wybrany będzie odpowiedni zestaw spritów
     */
    Animation* createAnimation(CharacterOnBoard* animatedObj,
                               Character::Activity activity);

    /**
     * tworzy animację z jednoczesnym przesunięciem na ekranie
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param move to akcja ruchu ze specjalnej grupy klas do akcji w animacjach
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationMove(CharacterOnBoard* animatedObj,
                                   Animations::Actions::Move move);

    /**
     * tworzy animację otrzymywania obrażeń, która redukuje punkty życia postaci
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param damage to ilość punktów obrażeń, jaką otrzyma postać podczas animacji
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationHurt(CharacterOnBoard* animatedObj, int damage);

    /**
     * tworzy animację umierania, która przestawia status postaci na umierający
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationDeath(CharacterOnBoard* animatedObj);

    /**
     * tworzy animację zniknięcia postaci, która wyłącza natychmiast pasek z hp
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationDisappear(CharacterOnBoard* animatedObj);

    // utworzenie nowego zbioru na równolegle wykonujące się animacje
    std::vector<Animation*>& addNewSet();

    // dodanie animacji do wskazanego zbioru równolegle wykonywanych animacji
    void addAnimationToSet(std::vector<Animation*>& set, Animation* animation);

    // dodanie animacji do kolejki (automatycznie tworząc dla niej nowy zbiór)
    void addAnimationToQueue(Animation* animation);

    // dodanie animacji do wektora równolegle wywoływanych animacji bezczynności
    void addIdleAnimation(Animation* animation);

    // wykonanie animacji dla bieżącej klatki
    void updateAnimationsStack(float delta);

    // update wszystkich animacji bezczynności
    void updateIdleAnimations(std::vector<CharacterOnBoard*> characters, float delta);

    // update wybranej animacj i zwrócenie wyniku o tym czy się zakończyła
    bool update_animation_and_check_if_finished(Animation& animation, float delta);

    // zwraca informację czy zostały jakieś animacje w kolejce (blokują one tryb gry)
    bool anyAnimationLocking();
  private:
    // kolejka zbiorów animacji czekających do wywołania
    std::queue<std::vector<Animation*>> sets_queue;

    // węzeł z równolegle wykonywanymi animacjami
    std::vector<Animation*> current_set;

    // równolegle wykonywane animacje bezczynności (nie blokują trybu gry)
    std::vector<Animation*> idle_animations;

    // klatka na której jest animacja dla wybranej postaci
    std::map<CharacterOnBoard*, float> current_frame;
  };
}

#endif /* ANIMATIONS_MANAGER_H_ */
