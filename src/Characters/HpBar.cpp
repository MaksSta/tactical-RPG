#include "HpBar.h"

HpBar::HpBar(sf::Vector2f pos, int& _hp, int& _max_hp)
  :
  hp{_hp},
  max_hp{_max_hp}
{
  hp_left_bar.setSize({50 * hp/static_cast<float>(max_hp), 12});

  setPosition(pos);

  // przygotowanie paska wyświetlającego AKTUALNE hp
  hp_left_bar.setSize({50, 12});
  hp_left_bar.setFillColor(sf::Color{185, 0, 0});
  hp_left_bar.setOutlineThickness(2);
  hp_left_bar.setOutlineColor(sf::Color{0, 0, 0});

  // stworzenie paska wyświetlającego CAŁKOWITE hp
  hp_max_bar.setSize({50, 12});
  hp_max_bar.setFillColor(sf::Color{0, 0, 0});
  hp_max_bar.setOutlineThickness(2);
  hp_max_bar.setOutlineColor(sf::Color{0, 0, 0});

  // przygotowanie paska wyświetlającego obecne obrażenia
  damage_bar.setFillColor(sf::Color{255, 255, 255});

  // przygotowanie tekstu z ilością hp
  hpText.setCharacterSize(12);
  hpText.setFillColor(sf::Color{40, 40, 40});
}

void HpBar::update(float deltaTime)
{
  // zablokowanie efektu skracania paska obrażeń na czas zamrożenia
  if (damage_effect_freeze > 0) {
      damage_effect_freeze -= deltaTime;
  }
  else
    // efekt otrzymania obrażeń w czasie na pasku obrażeń
    if (damage_effect > 0) {
      damage_effect -= deltaTime * 20;
      if (damage_effect < 0)
        damage_effect = 0;
    }

  damage_bar.setSize({damage_effect, 12});

  // aktualizacja obecnego i maksymalnego hp w napisie
  std::stringstream ss;
  ss << hp << "/" << max_hp;
  hpText.setString(ss.str());
}

void HpBar::takeDamage(float dmg)
{
  // ustawienie efektu zamrożenia
  damage_effect_freeze = 0.25;

  // ustawienie efektu otrzmywania obrażeń
  damage_effect = 50 * dmg/static_cast<float>(max_hp);

  // zmniejszenie wielkości paska obecnych hp do nowej wartości
  hp_left_bar.setSize({50 * hp/static_cast<float>(max_hp), 12});

  // umiejscowienie paska z efektem otrzymania obrażeń po prawej od pasku obecnych hp
  damage_bar.setPosition(hp_left_bar.getGlobalBounds().left + hp_left_bar.getGlobalBounds().width - 2,
                         hp_left_bar.getPosition().y);
}

void HpBar::setPosition(sf::Vector2f pos)
{
  hp_left_bar.setPosition(pos.x + 50/2,
                          pos.y);

  damage_bar.setPosition(hp_left_bar.getGlobalBounds().left + hp_left_bar.getGlobalBounds().width - 2,
                         hp_left_bar.getPosition().y);

  hp_max_bar.setPosition(pos.x + 50/2,
                         pos.y);

  // wyśrodkowanie tekstu
  hpText.setPosition(pos.x + (100 - hpText.getGlobalBounds().width)/2,
                     pos.y - 2);
}

void HpBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(hp_max_bar);
  target.draw(hp_left_bar);
  target.draw(damage_bar);
  target.draw(hpText);
}
