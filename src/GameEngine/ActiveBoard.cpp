#include "ActiveBoard.h"

void ActiveBoard::loadActiveFields(sf::Vector2i coordsTopLeft, FullBoard& fullBoard)
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      field[x][y] = fullBoard.getField(coordsTopLeft.x + x, coordsTopLeft.y + y);
}

void ActiveBoard::addCharacter(CharacterOnBoard* character)
{
  charactersOnBoard.push_back(character);
}

Field* ActiveBoard::getField(sf::Vector2i coords) const
{
  if (coords.x < 0
      || coords.x >= 8
      || coords.y < 0
      || coords.y >= 8)
    return nullptr;

  return field[coords.x][coords.y];
}

sf::Vector2i ActiveBoard::getCoordsOf(Field* checked_field) const
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      if (field[x][y] == checked_field)
        return sf::Vector2i{x, y};

  throw errors::field_not_found{};
}

Field* ActiveBoard::getFieldOccupedBy(CharacterOnBoard* character) const
{
  return getField(character->getLocalCoords());
}

CharacterOnBoard* ActiveBoard::getCharacterOnField(Field* field) const
{
  for (auto &character : charactersOnBoard)
    if (field == getFieldOccupedBy(character))
      return character;

  return nullptr;
}