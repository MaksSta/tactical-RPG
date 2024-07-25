#include "FullBoard.h"

FullBoard::FullBoard(std::string level_name)
{
	if (!level.loadMapFromFile(level_name))
	{
		throw errors::cannot_open_file{level_name};
	}
	
	for (int x = 0; x < max_board_width; x++)
		for (int y = 0; y < max_board_height; y++)
			field[x][y] = level.board[x][y].get();
}

Field* FullBoard::getField(int x, int y)
{
    return field[x][y];
}
    
int FullBoard::getWidth()
{
    return level.width;
}

int FullBoard::getHeight()
{
	return level.height;
}