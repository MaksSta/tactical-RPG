#include "Program.h"

Program::Program(sf::Vector2i window_size)
{
	window.create(	sf::VideoMode(window_size.x, window_size.y),
					"RPG",
					sf::Style::Close);

	// ustawienie maksymalnego dopuszczalnego fps
	window.setFramerateLimit(200);
}

void Program::run()
{
	/** 
	 * wewnątrz konstruktora klasy Game
	 * tworzone są wszystkie elementy gry
	 */
    Game game(
		window,
		window.getSize(), 	// całkowity rozmiar ekranu

		{0.02f, 0.03f},		// pozycja startowa rysowania planszy gry
							// wyrażona względnie jako ułamek wielkości całego okna

		{800, 800}			// rozmiar jaki stanowi plansza gry
	);
	
	/**
	 * tu znajduje się pełna pętla główna
	 */
    game.run();
}