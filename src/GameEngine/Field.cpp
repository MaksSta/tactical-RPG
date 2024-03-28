#include "Field.h"

// Field::Field() {}

Field::Field(sf::Vector2i _coords, TileType _type)
:
    coords{_coords},
    type{_type}
{
}

sf::Vector2i Field::getCoords() {
	return coords;
}

void Field::setCoords(sf::Vector2i new_coords) {
	coords = new_coords;
}
