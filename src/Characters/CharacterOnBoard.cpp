#include "CharacterOnBoard.h"

CharacterOnBoard::CharacterOnBoard(sf::Vector2i start_coords,
                                   std::string _name,
                                   Team _team,
                                   short _max_hp,
                                   sf::Vector2f _padding,
                                   float _scale)
  :
  Character(_name, _team, _max_hp),
  globalCoords{start_coords},
  padding{_padding},
  hpBar{getPosition(), hp, max_hp}
{
  sprite.setScale(_scale, _scale);

  localCoords = {globalCoords.x % 8,
                 globalCoords.y % 8};
}

void CharacterOnBoard::loadAnimation(Activity activity,
                                     Direction direction,
                                     std::string src_img,
                                     int frames,
                                     float fps,
                                     int img_w,
                                     int img_h)
{
  Sprites_data newOne;
  newOne.fps = fps;

  // ładowanie tekstur z pliku
  for (int i = 0; i < frames; i++) {
    sf::Texture t;
    t.loadFromFile(src_img, (sf::IntRect(sf::Vector2i(img_w * i, 0), sf::Vector2i(img_w, img_h))));
    newOne.textures.push_back(t);
  }

  sprites_data[{activity, direction}] = newOne;
}

Sprites_data& CharacterOnBoard::get_current_sprites_data()
{
  return sprites_data[{currentActivity, direction}];
}

void CharacterOnBoard::update(float deltaTime)
{
  // dodanie połowy paddingu
  sprite.setPosition(getPosition() - padding);

  hpBar.setPosition(getPosition());
  hpBar.update(deltaTime);
}

sf::Vector2i CharacterOnBoard::getLocalCoords() const
{
  return localCoords;
}

void CharacterOnBoard::setLocalCoords(sf::Vector2i new_coords)
{
  localCoords = new_coords;
}

sf::Vector2i CharacterOnBoard::getGlobalCoords() const
{
  return globalCoords;
}

void CharacterOnBoard::setGlobalCoords(sf::Vector2i new_coords)
{
  globalCoords = new_coords;
}

void CharacterOnBoard::reset_texture() {
  sprite.setTexture(get_current_sprites_data().textures[0]);
}

void CharacterOnBoard::takeDamage(int dmg)
{
  // ograniczonie otrzymanych obrażeń do pozostałej ilości hp
  int final_dmg = std::min(dmg, hp);
  hp -= final_dmg;

  // animacja na pasku hp
  hpBar.takeDamage(final_dmg);
}

std::vector<Attack>& CharacterOnBoard::getAttacks()
{
  return attack;
}

Button_data& CharacterOnBoard::getButtonData(Activity activity)
{
  return button_data_for_attack[activity];
}

Button_data & CharacterOnBoard::get_finish_turn_button()
{
  return finish_turn_button;
}

void CharacterOnBoard::init()
{
  currentActivity = Character::idle;
  direction = down;

  // ustawienie tekstury na startową
  reset_texture();

  // umiejscowienie postaci na ekranie
  setPosition(sf::Vector2f(getLocalCoords().x * tile_size,
                           getLocalCoords().y * tile_size));
}

void CharacterOnBoard::draw(sf::RenderTarget &target,
                            sf::RenderStates states) const
{
  // wyświetlenie postaci
  target.draw(sprite);

  // pasek hp nad postacią
  if (displayHpBar)
    target.draw(hpBar);
}

void CharacterOnBoard::move(sf::Vector2f offset)
{
  position += offset;
}

sf::FloatRect CharacterOnBoard::getGlobalBounds() const
{
  return sprite.getGlobalBounds();
}

sf::Vector2f CharacterOnBoard::getPosition() const
{
  return position;
}

void CharacterOnBoard::setPosition(sf::Vector2f pos)
{
  position = pos;
}

void CharacterOnBoard::setTransparency(unsigned char transparency)
{
  sprite.setColor({255, 255, 255, transparency});
}

void CharacterOnBoard::disableHpBar()
{
  displayHpBar = false;
}

void CharacterOnBoard::setTexture(sf::Texture& texture)
{
  sprite.setTexture(texture);
}
