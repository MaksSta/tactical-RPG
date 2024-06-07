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

    // rekurencyjne ponowne wywoływanie jeśli postać jest już martwa
    if (!getCurrentCharacter()->isAlive())
        switchToNextCharacter();
}

void BattleQueue::remove(CharacterOnBoard* character)
{
    for (std::deque<CharacterOnBoard*>::iterator it = queue.begin(); it != queue.end(); it++)
    {
        if (character == *it)
        {
            queue.erase(it);
            break;
        }
    }
}