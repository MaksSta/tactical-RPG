/**
 * Kolejka bitwy czyli lista kolejno wykonywujących po sobie ruch postaci
*/

#ifndef GAME_ENGINE_BATTLE_QUEUE_
#define GAME_ENGINE_BATTLE_QUEUE_

#include <deque>

#include "../Characters/CharacterOnBoard.h"

class BattleQueue {
public:
    // dodanie postaci na końcu kolejki bitwy
    void addToQueue(CharacterOnBoard* character);

    // zwraca postać której tura dzieje się obecnie
    CharacterOnBoard* getCurrentCharacter();

    // ustawienie kolejki bitwy na pierwszą postać, zalecane do wywołania na początku
    void setOnFirstCharacter();

    // przełączenie na następną podstać w kolejce bitwy
    void switchToNextCharacter();

    // usunięcie postaci z kolejki bitwy
    void remove(CharacterOnBoard* character);
private:
    std::deque<CharacterOnBoard*> queue;
    std::deque<CharacterOnBoard*>::iterator currentCharacter;
};

#endif /* GAME_ENGINE_BATTLE_QUEUE */