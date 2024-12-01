#ifndef MENU_MENU_H_
#define MENU_MENU_H_

#include <SFML/Graphics.hpp>

#include "MenuButton.h"
#include "../GameEngine/Game.h"
#include "../Fonts.h"

class Menu {
public:
  Menu(sf::RenderWindow &_window,
       sf::Vector2i window_size);

  // uruchomienie menu gry
  void run();

  void play();
  void quit();
private:
  bool gameStarted{false};

  // wskaźnik do okna gry
  sf::RenderWindow* window;

  // wymiary całego okna
  const sf::Vector2u window_size;

  sf::Event event;
};

#endif /* MENU_MENU_H_ */
