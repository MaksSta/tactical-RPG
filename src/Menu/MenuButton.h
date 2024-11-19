#ifndef MENU_MENUBUTTON_H_
#define MENU_MENUBUTTON_H_

#include <SFML/Graphics.hpp>

class Menu;

class MenuButton : public sf::Drawable {
public:
  MenuButton(std::string str,
             void (Menu::*method)(),
             float x,
             float y,
             float w,
             float h);

  sf::FloatRect getGlobalBounds() const;

  friend class Menu;
private:
  void draw(sf::RenderTarget &target,
            sf::RenderStates states) const;


  // metoda klasy Menu wywoływana przez ten przycisk
  void (Menu::*onClick)();

  sf::Font font;

  sf::RectangleShape background;
  sf::Text text;
};

#endif /* MENU_MENUBUTTON_H_ */
