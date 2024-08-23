/**
 * Pełna plansza gry, do załadowania z pliku
 */

#ifndef GAME_ENGINE_FULLBOARD_H_
#define GAME_ENGINE_FULLBOARD_H_

#include <string>

#include "Field.h"
#include "Level.h"

class FullBoard {
public:
  FullBoard(std::string level_name);

  Field* getField(int x, int y);

  int getWidth();
  int getHeight();
private:
  Level level{"data/tileset.png", 9};
  Field* field[max_board_width][max_board_height];
};

namespace errors
{
  struct cannot_open_file {std::string filename;};
}

#endif /* GAME_ENGINE_FULLBOARD */
