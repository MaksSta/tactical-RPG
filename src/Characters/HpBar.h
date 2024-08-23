/**
 * Pasek wyświetlający stan życia dla postaci
 * zostaje zainicjowany z referencjami do danych o hp
 * postaci nad którą ma się wyświetlać
 */

#ifndef CHARACTERS_HPBAR_H_
#define CHARACTERS_HPBAR_H_

#include <SFML/Graphics.hpp>
#include <sstream>

#include "../UI/Textfield.h"

class HpBar : public sf::Drawable {
public:
  /**
   * Uworzenie paska z wyświetlany punktami życia
   * \param pos pozycja paska na ekranie
   * \param _hp referencja do aktualnego hp postaci
   * \param _max_hp referencja do maksymalnego hp postaci
   */
  HpBar(sf::Vector2f pos,
        int& _hp,
        int& _max_hp);

  void update(float deltaTime);

  // wywołanie efektu otrzymania obrażeń na pasku
  void takeDamage(float dmg);

  void setPosition(sf::Vector2f pos);
  void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  friend class CharacterOnBoard;
private:
  // referencja do aktualnego hp postaci
  int& hp;

  // referencja do maksymalnego hp postaci
  int& max_hp;

  // animowany efekt otrzymywania obrażeń na pasku życia w czasie
  float damage_effect{0.0f};

  // efekt zamrożenia obrażeń na pasku na czas podany w sekundach
  float damage_effect_freeze;

  // paski składające się na całóść paska życia
  sf::RectangleShape hp_left_bar, hp_max_bar, damage_bar;

  // tekst z informacją o punktach życia wyświetlany na środku paska
  Textfield hpText;
};

#endif /* CHARACTERS_HPBAR_H_ */
