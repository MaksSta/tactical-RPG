#include "Program.h"

Program::Program(sf::Vector2i window_size)
{
  window.create(sf::VideoMode(window_size.x, window_size.y),
                "Tactical RPG",
                sf::Style::Close);

  // ustawienie maksymalnego dopuszczalnego fps
  window.setFramerateLimit(200);
}

void Program::run()
{
  loadFonts();

  Menu menu(window,
            {1600, 900});
  menu.run();
}
