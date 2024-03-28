/**
 * Plansza gry,
 * gdzie zostają utworzone jej pola
 * 
 * Zawiera metody do wczutania pliku tekstur (konstruktor)
 * i metodą generującą pola zczytując je bezpośrenio z pliku txt
*/

#ifndef GAME_ENGINE_LEVEL_H_
#define GAME_ENGINE_LEVEL_H_

#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "Field.h"
#include "../Global.h"

const unsigned short max_board_width = 256;      
const unsigned short max_board_height = 256;

class Level {
public:
	// konstruktor wczytuje od razu pliku tekstur
	Level(std::string, unsigned int);

	// załadowanie planszy z pliku tekstowego
	bool loadMapFromFile(std::string level_name);
private:
	// tablica na tekstury kafelków
	sf::Texture texture[Field::tiles_count];

	// tablica zawierająca elementy planszy w formacie [x][y]
	std::unique_ptr<Field> board[max_board_width][max_board_height];

	// wymiary całej planszy (rzeczywisty rozmiar powyższej tablicy)
	unsigned short width, height;

	friend class Game;
};

#endif /* GAME_ENGINE_LEVEL */