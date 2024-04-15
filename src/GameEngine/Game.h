/**
 * Główna klasa gry w której zawiera się jej pełna mechanika
 * 
 * Najpierw należy utworzyć jej instancję, konstruktor ładuje wszystkie elementy
 * Następnie wywołanie run() powoduje uruchomienie pętli głównej
*/

#ifndef GAME_ENGINE_GAME_H_
#define GAME_ENGINE_GAME_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <array>

#include "Level.h"
#include "Pathfinder.h"
#include "Field.h"
#include "Camera.h"
#include "Road.h"
#include "Range.h"
#include "../UI/UI.h"
#include "../UI/TextBox.h"
#include "../Abilities/Attack.h"
#include "../Animations/Manager.h"
#include "../Characters/Warrior.h"
#include "../Characters/Sorceress.h"
#include "../Global.h"

class Game {
public:
	// tryb aktywności w jakim znajduje się gra
	enum GameMode {
		locked_player_turn,
		player_turn,
	};

	// dodatkowy trybu aktywności, który szczegółowo decyduje o sposobie wczytywania danych wejściowych
	enum InputMode {
		none,
		character_is_selected,
		action_is_selected,
	};

	/** 
	 * stworzenie okna gry, inicjalizacja wszystkich elementów gry,
	 * załadowanie grafik i planszy
	 * \param window referencja do okna sfml
	 * \param _screen_size rozmiar całego okna ekranu
	 * \param board_starts_at miejsce na ekranie w którym zaczyna się plansza [jako ułamek całego rozmiaru ekranu]
	 * \param board_screen_size rozmiar jaki stanowi plansza gry [w pikselach]
	*/
	Game(	sf::RenderWindow &window,
	 		sf::Vector2u _screen_size,
			sf::Vector2f board_starts_at,
			sf::Vector2f board_screen_size);

	// uruchomienie gry - wewnątrz znajduje się pętla główna z całą logiką
	void run();
private:
	void update(float deltaTime);

	/**
	 * dotyczy podglądu akcji tworzącego się dynamicznie przez wskazywanie myszką pól na planszy
	 * (nie wymaga wcześniejszego wybieranie akcji do wywołania)
	 * 
	 * sprawdzana jest możliwość wykonania ruchu, jeśli ok tworzony jest jej podgląd
	 * następnie wewnątrz zostaje wywołane checkActionsByHover(), by sprawdzić czy można
	 * wywołać również dodatkową akcję poza ruchem
	*/
	void checkMoveAndActionsAuto();
	
	/**
	 * sprawdzana jest możliwość wykonania automatycznie wybranej akcji na wskazywanym polu 
	 * 
	 * jeśli ok tworzony jest jej podgląd
	 * może przy okazji usunąć obecny podgląd drogi (np. przy ataku z dystansu)
	*/
	void checkActionsByHover();

	// akceptuje wywołania ruchu i/lub akcji
	void acceptMoveAndAction();

	// ustawia postać na nowym polu, wywołuje animację ruchu
	void acceptMovePlayer();

	// wywołanie akcji podanego ataku w miejscu na podstawie podglądu range
	void acceptAttack(Attack& attack);

	// utworzonenie podglądu range względem zaznaczonej postaci na podstawie podanych przesunięć względem jej
	void createRangePreview(std::vector<sf::Vector2i> in_range);

	// sprawdzenie czy na wskazywanym polu znajduje się inna postać
	bool isEnemyOnHoveredField();

	// uzyskanie pola podając współrzędne kafelka z całej mapy
	Field* get_active_field_from_absolute_coords(sf::Vector2i);	

	// przełącza tryb gry na zablokowany (by wywołać całą kolejkę animacji)
	void lockGameMode();

	// przełącza tryb gry na odlbokowany, przełącza justUnlocked na true
	void unlockGameMode();

	/**
	 * sprawdzanie czy klinięto podany przycisk myszy
	 * odbywa się przez wywołanie funkcji sfml isButtonPressed
	 * \param mouse_btn nazwa przycisku myszy z sfml
	*/
	bool mouseClicked(sf::Mouse::Button mouse_btn);

	/**
	 * sprawdzanie czy klinięto podany przycisk myszy i jej kursor znajduje się w odpowiednim miejscu
	 * odbywa się przez wywołanie funkcji sfml isButtonPressed
	 * \param mouse_btn nazwa przycisku myszy z sfml
	 * \param objRect prostokąt stanowiący obszar w środku którego trzeba kliknąć
	 * \param m_pos pozycja kursora myszy
	*/
	bool mouseClicked(sf::Mouse::Button mouse_btn, sf::FloatRect objRect, sf::Vector2f m_pos);

	// czy wykryto zmianę ostatnio wskazywanego pola na planszy
	bool hovered_field_changed();

	// tryb aktywności w jakims znajduje się gra
	GameMode gameMode;

	// dodatkowy trybu aktywności, który decyduje o tym jakie czytać wydarzenia z myszy/klawiatury
	InputMode inputMode {none};

	/** 
	 * obiekt planszy, w którym zostaną utworzone pola tworące planszę
	 * wywołanie konstruktora powoduje pobranie tilesetu
	 * w argumentach ścieżka pliku tekstur i liczba tekstur do załadowania
	*/ 
	Level level{"data/tileset.png", 9};

	// zbiór wszystkich postaci tworzony inteligentymi wskaźnikami
	std::vector<std::unique_ptr<CharacterOnBoard>> charactersOnBoard;

	// zaznaczona obecnie postać
	CharacterOnBoard* selectedCharacter {nullptr};

	// wskazywane myszką obecne pole
	Field* hoveredField {nullptr};

	// pogląd ścieżki do przejścia akcją ruchu 
	Road road;

	// podgląd ataku do wykoniania akcją ataku
	Range range; 

	// informacja czy podgląd akcji do wykonania pochodzi z autoataku
	bool range_created_from_auto;
	
	// ilość akcji jaka zostanie w przypadku wykonania wskazywanej akcji
	int AP_preview;

	// tekstu który wyświetla podgląd ile punktów zostanie po wykonaniu akcji
	Textfield text_AP_preview;

	// współrzędne pola na planszy od których zaczyna się aktywny obszar gry
	sf::Vector2i coordsTopLeft {0, 0};
	
	// pola na aktywnym obszarze gry, czyli widocznym kadrze planszy
	Field* activeField[8][8];

	// interejs użytkownika, zaczynający się w miejscu od podanych współrzędnych
	UI ui{{880, 475}};

	// czy w ostatniej klatce wciśnięto LPM
	bool MouseLClickedLastFrame;

	// informacja sprawdzana co klatkę, by 1razowo odświeżyć niektóre elementy
	bool justUnlocked;

	// ostatnio najeżdzane pole na planszy
	Field* lastHoveredField {nullptr};

	// pozycja kursora myszy przeliczona na pozycję w widoku ma kamerze
	sf::Vector2f m_pos_on_map;

	// standardowa pozyja kursora myszy liczona od krawędzi okna
	sf::Vector2f m_pos_on_window;

	/********************************************************************/
	/**************** ANIMACJE - WSZYSTKO BY NIMI ZARZĄDZAĆ *************/
	/********************************************************************/
	Animations::Manager anim_manager;

	/********************************************************************/
	/****************** ELEMENTY GRAFICZNE I RYSOWANIE ******************/
	/********************************************************************/
	// kamera pokazująca obecną część planszy na ekranie
	Camera camera;

	// podświetlenie wskazywanego myszką pola
	sf::RectangleShape hoveredRect;

	// trójkącik, który animuje się nad zaznaczoną postacią
	sf::CircleShape indicator_selected_character{9, 3};
	float indicator_modifier {0.0f};
	bool indicator_frame_raise {true};

	// metoda do rysowania wszystkich obiektów planszy
	void draw_board();

	// metoda do rysowania obiektów stale widocznych na ekranie
	void draw_static_elements();

	// tło całego ekranu
	sf::RectangleShape background;

	// tło na planszę (pod jej spodem, więc stanowi obwódkę)
	sf::RectangleShape borderForGameScreen;

	/********************************************************************/
	/******************** SFML - ELEMENTY WBUDOWANE *********************/
	/********************************************************************/
	// wymiary całego okna gry
	const sf::Vector2u screen_size;

	// wskaźnik do okna gry
	sf::RenderWindow* window;

	// od obsługi zdarzeń wejścia
	sf::Event event;

	// zegar z obecnym czasem
	sf::Clock time;
};

#endif /* GAME_ENGINE_GAME_H_ */