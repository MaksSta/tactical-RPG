#ifndef FIELD_H_
#define FIELD_H_

#include <SFML/Graphics.hpp>

class Field : public sf::Sprite {
public:
	enum TileType {
		floor,
		floor_L_wall,
		floor_T_wall,
		floor_B_wall,
		floor_R_wall,
		floor_TL_wall,
		floor_TR_wall,
		floor_BL_wall,
		floor_BR_wall,
		tiles_count
	};

    Field(sf::Vector2i _coords, TileType _type);	

	sf::Vector2i getCoords();
   	void setCoords(sf::Vector2i new_coords);
protected:
	sf::Vector2i coords;
    TileType type;
};

#endif /* FIELD_H_ */