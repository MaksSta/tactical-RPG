#include "ActiveBoard.h"

ActiveBoard::ActiveBoard()
{}

void ActiveBoard::loadActiveFields(sf::Vector2i coordsTopLeft, FullBoard& fullBoard)
{
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			field[x][y] = fullBoard.getField(coordsTopLeft.x + x, coordsTopLeft.y + y);
}

Field* ActiveBoard::getFieldByLocalCoords(sf::Vector2i coords)
{
	if (	coords.x < 0
		||	coords.x >= 8
		|| 	coords.y < 0
		||	coords.y >= 8
	)
	return nullptr;

	return field[coords.x][coords.y];
}

Field* ActiveBoard::getFieldFromAbsoluteCoords(sf::Vector2i coords)
{
    return field[coords.x - globalCoordsOfTopLeft.x][coords.y - globalCoordsOfTopLeft.y];
}