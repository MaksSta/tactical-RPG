#ifndef AI_CORE_H_
#define AI_CORE_H_

#include "../GameEngine/ActiveBoard.h"
#include "../GameEngine/Pathfinder.h"
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

    bool any_action_taken;
    int AP;

    void try_attack();
    void try_move_and_attack();
    void calculate_possible_moves();
    void move_to_the_center();
    void follow_closest_enemy();

    std::vector<std::vector<sf::Vector2i>> legal_offsets;
  };
}

#endif /* AI_CORE_H_ */
