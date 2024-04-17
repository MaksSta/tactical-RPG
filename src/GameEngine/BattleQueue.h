#ifndef GAME_ENGINE_BATTLE_QUEUE_
#define GAME_ENGINE_BATTLE_QUEUE_

#include <deque>

#include "../Characters/CharacterOnBoard.h"

class BattleQueue {
public:
    void addToQueue(CharacterOnBoard* character);

    CharacterOnBoard* getCurrentCharacter();

    void setOnFirstCharacter();

    void switchToNextCharacter();
private:
    std::deque<CharacterOnBoard*> queue;
    std::deque<CharacterOnBoard*>::iterator currentCharacter;
};

#endif /* GAME_ENGINE_BATTLE_QUEUE */