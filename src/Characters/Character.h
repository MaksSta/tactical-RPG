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
	// lista aktywności jaką może przybrać postać
	enum Activity {
		idle,
		move,
		attack1,
		attack2,
		attack3,
		hurt,
	};

	/**
	 * Utworzenie wstępnej klasy postaci
	 * \param _name nazwa postaci do wyświetlenia
	 * \param _max_hp maksymalna i zarazem początkowa ilość punktów życia
	*/
    Character(std::string _name, int _max_hp);

	Activity get_activity();
	void setActivity(Activity);

	Direction getDirection();
	void setDirection(Direction);

	int getAP();
	void setAP(int);

	int getHP();
	void setHP(int);

	std::string getName();
protected:
	Activity currentActivity {idle};
	Direction direction {down};

	std::string name;
	int hp;
	int max_hp;

	int max_action_points, action_points;  
};

#endif /* CHARACTERS_CHARACTER_H_ */