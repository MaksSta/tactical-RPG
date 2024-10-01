#ifndef AI_CORE_H_
#define AI_CORE_H_

#include "../GameEngine/ActiveBoard.h"
#include "Decision.h"

namespace AI
{
  class Core {
  public:
    Core(CharacterOnBoard* _controlledCharacter,
         ActiveBoard& _board);

    Decision calculateBestDecision();
    AttackInfo calculateBestAttack(int AP,
                                   sf::Vector2i offset_from_character_pos = {0, 0});

    CharacterOnBoard* tryFindTargetWithLowestHP(Range& range);
  private:
    Decision decision;

    ActiveBoard* board;
    CharacterOnBoard* controlledCharacter;
  };
}

#endif /* AI_CORE_H_ */