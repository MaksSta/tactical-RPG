/**
 * Uruchomienie programu, tu ma początek utworzenie okna przez sfml
 * referencja do okna jest przekazywana dalej
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <SFML/Graphics.hpp>

#include "Menu/Menu.h"

class Program {
public:
  Program(sf::Vector2i window_size);

  // uruchomienie programu
  void run();
private:
  sf::RenderWindow window;
};

#endif /* PROGRAM_H_ */
