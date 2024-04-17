#ifndef GAME_ENGINE_BATTLE_QUEUE_
#define GAME_ENGINE_BATTLE_QUEUE_

#include <deque>

#include "../Characters/CharacterOnBoard.h"

class BattleQueue {
public:
    void addToQueue(CharacterOnBoard* character);

    CharacterOnBoard* getCurrentCharacter();

    void setOnFirstCharacter();
private:
    std::deque<CharacterOnBoard*> queue;
    std::deque<CharacterOnBoard*>::iterator currentCharacter;
    // std::queue<CharacterOnBoard*>::
};

#endif /* GAME_ENGINE_BATTLE_QUEUE */