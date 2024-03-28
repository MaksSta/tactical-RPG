#include "Level.h"

Level::Level(std::string tile_file, unsigned int number_of_textures)
{
	for (int i = 0; i < number_of_textures; i++)
	{
		texture[i].loadFromFile(tile_file,
								sf::IntRect(sf::Vector2i(tile_size * i, 0),
											sf::Vector2i(tile_size, tile_size)));
	}
}

bool Level::loadMapFromFile(std::string level_name)
{
	std::fstream file;

	file.open("data/" + level_name);

	if (!file.is_open())
		return false;

	// wczytanie rozmiaru planszy z nagłówka pliku
	file >> width >> height;

	if (width == 0 || height == 0) {
		file.close();
		return false;
	}

	// stworzenie pól bezprośrednio ładując dane z pliku
	for (int y = 0; y < height; y++) 
		for (int x = 0; x < width; x++) 
		{
			short buffer;

			file >> buffer;

			 board[x][y] = std::make_unique<Field>(
				sf::Vector2i{x, y},
				static_cast<Field::TileType>(buffer)
			);

			board[x][y]->setPosition(x * tile_size, y * tile_size);
			board[x][y]->setTexture(texture[buffer]);
		}

	file.close();

	return true;
}


