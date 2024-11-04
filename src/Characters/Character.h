/**
 * Bazowa klasa postaci
 * zawiera informację o:
 *  - aktywności i kierunku
 *  - ilościach obecnych i maksymalnych punktów życia (HP)
 *  - ilościach obecnych i maksymalnych punktów akcji (AP)
 *  - nazwę postaci
 */

#ifndef CHARACTERS_CHARACTER_H_
#define CHARACTERS_CHARACTER_H_

#include <string>

#include "../Global.h"

class Character {
public:
  // lista aktywności jaką może przybrać postać
  enum Activity {
    idle,
    move,
    attack1,
    attack2,
    attack3,
    hurt,
    death,
  };

  enum Team {
    player,
    enemy
  };

  /**
   * Utworzenie wstępnej klasy postaci
   * @param _name nazwa postaci do wyświetlenia
   * @param _team przez kogo sterowana jest postać
   * @param _max_hp maksymalna i zarazem początkowa ilość punktów życia
   */
  Character(std::string _name,
            Team _team,
            int _max_hp);

  Activity getActivity() const;
  void setActivity(Activity);

  Direction getDirection() const;
  void setDirection(Direction);

  bool isAlive() const;

  Team getTeam() const;

  int getAP() const;
  void setAP(int);
  int getMaxAP() const;

  int getHP() const;
  void setHP(int);

  std::string getName() const;

  // informacja o tym, że postać zaraz umrze (animacja śmierci czeka już w kolejce)
  bool will_die_this_turn {false};
protected:
  Team team;

  Activity currentActivity{idle};
  Direction direction{down};

  std::string name;
  int hp;
  int max_hp;

  int max_action_points, action_points;
};

#endif /* CHARACTERS_CHARACTER_H_ */
