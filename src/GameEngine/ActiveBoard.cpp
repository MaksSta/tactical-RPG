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

bool ActiveBoard::isFieldInRange(Field* field,
                                 Range& range)
{
  for (auto &r : range.get())
    if (r == field)
      return true;

  return false;
}

Range ActiveBoard::createRange(CharacterOnBoard* character,
                               std::vector<sf::Vector2i> in_range)
{
  std::vector<Field*> vec;

  // znalezienie pól będacych w zasięgu
  for (auto &r : in_range) {
    Field* af =
      getField(getCoordsOf(getFieldOccupedBy(character))
               + r);
    
    if (af != nullptr)
      vec.push_back(af);
  }

  Range range(vec, getFieldOccupedBy(character));

  return range;
}

std::vector<Field*> ActiveBoard::getBlockedFields() const
{
  std::vector<Field*> blockedFields;

  // znalezienie wszystkich pól gdzie stoi już inna, żywa postać
  for (auto &character : getAliveCharacters()) {
    auto af = getFieldOccupedBy(character);
    blockedFields.push_back(af);
  }
  
  return blockedFields;
}

std::vector<CharacterOnBoard*> ActiveBoard::getAliveCharacters() const
{
  std::vector<CharacterOnBoard*> aliveCharacters;
  for (auto &character : charactersOnBoard)
    if (character->isAlive())
      aliveCharacters.push_back(character);

  return aliveCharacters;
}