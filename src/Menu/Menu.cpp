#include "Menu.h"

Menu::Menu(sf::RenderWindow &_window,
           sf::Vector2i window_size)
  :
  window{&_window},
  window_size{window_size}
{}

void Menu::run()
{
  sf::RectangleShape loadingScreenBackground{static_cast<sf::Vector2f>(window_size)};
  loadingScreenBackground.setPosition(0.0f, 0.0f);
  loadingScreenBackground.setFillColor(sf::Color{0,15,175});

  sf::Text loadingScreenText("Loading game...", font_normal, 88);
  loadingScreenText.setOrigin(loadingScreenText.getLocalBounds().getPosition());
  loadingScreenText.setPosition(window_size.x / 2 - loadingScreenText.getGlobalBounds().width / 2,
                                window_size.y / 2 - loadingScreenText.getGlobalBounds().height / 2);
  loadingScreenText.setFillColor(sf::Color{215,215,0});

  MenuButton play_btn("PLAY",
                      &Menu::play,
                      window_size.x / 2,
                      window_size.y / 2 - 100,
                      650.f,
                      140.f);

  MenuButton quit_btn("QUIT",
                      &Menu::quit,
                      window_size.x / 2,
                      window_size.y / 2 + 100,
                      650.f,
                      140.f);

  std::vector<MenuButton*> buttons;
  buttons.push_back(&play_btn);
  buttons.push_back(&quit_btn);

  while (window->isOpen())
  {
    auto m_pos_on_window = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

    while (window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window->close();

      for (auto & menu_button : buttons)
      {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            menu_button->getGlobalBounds().contains(m_pos_on_window))
        {
          // wywołanie metody z tej klasy do której odnosi się konkretny przycisk
          ((*this).*(menu_button->onClick))();
        }
      }
    }

    window->clear();

    window->draw(play_btn);
    window->draw(quit_btn);

    // zasłonięcie całego ekranu widokiem ładowania gry
    if (gameStarted)
    {
      window->draw(loadingScreenBackground);
      window->draw(loadingScreenText);
    }

    window->display();

    // uruchomienie pętli głównej gry wewnątrz
    if (gameStarted)
    {
      Game game(*window,
                sf::Vector2u{window_size.x,window_size.y}, // całkowity rozmiar ekranu
                {0.02f, 0.03f},   // pozycja startowa rysowania planszy, wyrażona względnie jako ułamek wielkości całego okna
                {800, 800}        // rozmiar jaki stanowi plansza gry
        );
      game.run();
    }

  }

}

void Menu::play()
{
  // uruchomienie rozgrywki przed kolejną iteracji pętli gł.
  gameStarted = true;
}

void Menu::quit()
{
  window->close();
}
