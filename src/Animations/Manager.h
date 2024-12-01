/**
 *  Klasa ta obsługuje animację według koncepcji:
 *
 *  animacje wykonywane kolejką jedna po drugiej to sekwencja
 *  równolegle wykonywane jest wiele ścieżek
 *
 *  cyfry przedstawieniają kolejno wykonywane sekwnecje,
 *  a literki każdą ze ścieżek
 *   === oznacza czas trwania animacji
 *  przykładowo jak to może wyglądać dla
 *  ---------------------------------> Czas
 *  [A1==][A2==][A3==]
 *       [B1====][B2===]
 *                      [C1===]
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

#include "Animation.h"
#include "MoveBy.h"
#include "Hurt.h"
#include "Death.h"
#include "Disappear.h"
#include "ChangeHpBar.h"

#include "Actions/Move.h"

namespace Animations
{
  class Sequence : public std::vector<Animation*> {
  public:
    sf::Time timeToStart;

    sf::Clock stoper;

    Sequence() {
      stoper.restart();
    }
  };

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
                                   Animations::Actions::Move move,
                                   bool isBlocking);

    /**
     * tworzy animację otrzymywania obrażeń, która redukuje punkty życia postaci
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param damage to ilość punktów obrażeń, jaką otrzyma postać podczas animacji
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationHurt(CharacterOnBoard* animatedObj);

     /**
     * tworzy animację otrzymywania obrażeń, która odejmuje punkty życia z paska
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @param damage to ilość punktów obrażeń, jaką otrzyma postać podczas animacji
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationTakeDamageHP(CharacterOnBoard* animatedObj,
                                           int damage);

    /**
     * tworzy animację umierania, która przestawia status postaci na umierający
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @return zwraca wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationDeath(CharacterOnBoard* animatedObj);

    /**
     * tworzy animację zniknięcia postaci, która wyłącza natychmiast pasek z hp
     * @param animatedObj wskaźnik do postaci na której wykonywana będzie animacja
     * @return wskaźnik do nowo utworzonej animacji
     */
    Animation* createAnimationDisappear(CharacterOnBoard* animatedObj);

    /**
     * utworzenie nowego zbioru na równolegle wykonujące się animacje
     * @return referencja do pustej sekwencji, która też jest nowo tworzona w całym zbiorze
     */
    Sequence& addNewSetParallel();

    // dodanie animacji do kolejki (automatycznie tworząc dla niej nowy zbiór)
    // obecnie nieużywam, bo to jest to samo co metoda wyżej do której robie push_back()
    void addInNewSetAndBlock(Animation* animation);

    // podaje czas do ukończenia podanej ścieżki animacji
    int calculateTimeLeft_ms(Sequence& sequence,
                             CharacterOnBoard* character = nullptr);

    // podaje czas do ukończenia wszystkich animacji
    int calculateTimeToLastAnimationFinish(CharacterOnBoard* character = nullptr);

    Sequence& addNewSequenceParallelAfterTime(int time_in_ms);

    // TODO do napisania: dodoje animację jako następną po wybranej
    void addAfterThatAnimation(Animation* animation);

    // dodanie animacji do wektora równolegle wywoływanych animacji bezczynności
    void addIdleAnimation(Animation* animation);

    // wykonanie wszystkich animacji dla bieżącej klatki
    void updateAnimationsStack(float delta);

    // update wszystkich animacji bezczynności
    void updateIdleAnimations(std::vector<CharacterOnBoard*> characters,
                              float delta);

    // update wybranej animacj i zwrócenie wyniku o tym czy się zakończyła
    bool update_animation_and_check_if_finished(Animation& animation,
                                                float delta);

    // zwraca informację czy zostały jakieś animacje w kolejce (blokują one tryb gry)
    bool anyAnimationLocking();

    bool anyAnimationLeft();
  private:
     //  wszystkie animacje, umieszczone w równolegle wykonywanych ścieżkach
    std::vector<Sequence> parallel_played_sequences;

    // równolegle wykonywane animacje bezczynności (nie blokują trybu gry)
    std::vector<Animation*> idle_animations;

    // klatka na której jest animacja dla podanej z postaci
    std::map<CharacterOnBoard*, float> current_frame;
  };
}

#endif /* ANIMATIONS_MANAGER_H_ */
