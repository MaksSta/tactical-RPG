#include "BattleQueue.h"

void BattleQueue::addToQueue(CharacterOnBoard* character)
{
    queue.push_back(character);
}

CharacterOnBoard* BattleQueue::getCurrentCharacter()
{
    return *currentCharacter;
}

void BattleQueue::setOnFirstCharacter()
{
    currentCharacter = queue.begin();
}

void BattleQueue::switchToNextCharacter()
{
    currentCharacter++;

    // po przekroczeniu zakresu wraca na początkową postać
    if (currentCharacter == queue.end())
        currentCharacter = queue.begin();
}