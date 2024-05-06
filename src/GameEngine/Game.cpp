#include "Game.h"

Game::Game(	sf::RenderWindow & _window,
			sf::Vector2u _screen_size,
		   	sf::Vector2f board_starts_at,
			sf::Vector2f board_screen_size)
:
	window{&_window},
	screen_size{_screen_size},
	camera{
		board_screen_size,
		sf::FloatRect {
			board_starts_at.x,
			board_starts_at.y,
			board_screen_size.x / _screen_size.x,
			board_screen_size.y / _screen_size.y}
		}
{
	// tło całego ekranu
	background.setFillColor(sf::Color{75, 75, 75});
	background.setSize(static_cast<sf::Vector2f>(screen_size));

	// ramka wyświetla dookoła obszaru planszy (technicznie tło pod spodem)
	const short border_size = 9;
	borderForGameScreen.setFillColor(sf::Color{70, 55, 55});
	borderForGameScreen.setSize({board_screen_size.x + border_size * 2,
								 board_screen_size.y + border_size * 2});
	borderForGameScreen.setPosition({board_starts_at.x * screen_size.x - border_size,
									 board_starts_at.y * screen_size.y - border_size});

	// załadowanie planszy do gry z pliku
	if (!level.loadMapFromFile("example_level.txt"))
	{
		std::cerr << "Error while loading map";
		return;
	}
	
	// ustawienie wielkości całej mapy
	camera.set_map_size(sf::Vector2f(	tile_size * level.width,
								 		tile_size * level.height));

	// ustawienie aktywnych pól planszy
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			activeField[x][y] = level.board[coordsTopLeft.x + x][coordsTopLeft.y + y].get();


	// utworzenie postaci
	charactersOnBoard.push_back(std::make_unique<Warrior>(sf::Vector2i{4, 4}));
	charactersOnBoard.push_back(std::make_unique<Sorceress>(sf::Vector2i{6, 6}));

	// dodanie postaci do kolejki bitwy
	for (auto & character : charactersOnBoard)
		battle_queue.addToQueue(character.get());

	// ustawienie tury na pierwszą postać z kolejki
	battle_queue.setOnFirstCharacter();
	selectCharacter(battle_queue.getCurrentCharacter());

	// dodanie animacji bezczynności (idle) do każdej postaci
	for (auto & character : charactersOnBoard)
	{
		anim_manager.addIdleAnimation (
			anim_manager.createAnimation(
				character.get(),
				Character::Activity::idle
			)
		);
	}

	// graficzne ustawienia dla znacznika zaznaczonej postaci
	indicator_selected_character.setFillColor(sf::Color{0, 135, 0});
	indicator_selected_character.setRotation(180);
}

void Game::run()
{
	gameMode = player_turn;

	// tu zapisywany będzie czas narysowania ostatniej klatki ekranu
	sf::Time lastUpdate = time.getElapsedTime();

	// początkowe ustawienie kamery
	camera.setCenter(8 * tile_size / 2 * 1,
					 8 * tile_size / 2 * 1);
	camera.setDstPos(camera.getCenter());

	// ********************* PĘTLA GŁÓWNA GRY *********************
	while (window->isOpen())
	{
		// ********************************************************************
		// ****** Niezbędne operacje do odświeżania w każdej klatce gry *******
		// ********************************************************************

		// pozycja myszy w całym oknie
		m_pos_on_window = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

		// pozycja myszy przekonwertowana na pozycje w obszarze ekranu aktywnego gry
		m_pos_on_map = window->mapPixelToCoords(sf::Mouse::getPosition(*window), camera);

		// znalezienie pola pod którym jest myszka
		hoveredField = nullptr;
		for (int x = 0; x < 8; x++)
			for (int y = 0; y < 8; y++)
			{
				auto f = activeField[x][y];
				if (f->getGlobalBounds().contains(m_pos_on_map)) 
					hoveredField = f;
			}
		// jeśli znaleziono, zostanie ono podświetlone
		if (hoveredField)
		{
			hoveredRect.setFillColor(sf::Color{0, 20, 255, 16});
			hoveredRect.setSize(sf::Vector2f(tile_size, tile_size));
			hoveredRect.setPosition(hoveredField->getPosition());
		}
				
		// ******************************************************************
		// ****** Obsługa wszystkich wydarzeń z myszy i klawiatury **********
		// ******************************************************************
		while (window->pollEvent(event) || justUnlocked)
		{
			justUnlocked = false;

			/****************************************************************
			 * Tryb gry: 		KAŻDY
			 * Pobieranie inputu odblokowanego we wszystkich trybach
			 * te operacje są stale sprawdzane, gdy wychwycono jakiś event
			 * 
			 * Znajduje się tu obsługa okna gry, interakcje z GUI itp.
			 /****************************************************************/
			// systemowe zamknięcie aplikacji (np. kliknięcie X na pasku okna / alt+f4)
			if (event.type == sf::Event::Closed)
				window->close();
	
			// akcje kliknięcia myszą dla elementów UI - PPM
			// możliwe do wykonania w każdej fazie gry
			for (auto & b: ui.button)
			{
				if (mouseClicked(sf::Mouse::Right) && b->getGlobalBounds().contains(m_pos_on_window))
					// jeżeli liczba akcji jest wystarczająca by wykonać tę akcję, zaznacza przycisk
					if (selectedCharacter->getAP() >= b->getAbility()->getAP())
						ui.autoselectButton(b.get());
			}

			switch(gameMode) {
			/****************************************************************
			 * Tryb gry: 		AKTYWNA TURA GRACZA
			/****************************************************************/
			case GameMode::player_turn:
			{
				switch(inputMode) {
				/****************************************************************
				 * Tryb inputu: 		ZAZNACZONA POSTAĆ, CZEKA NA POLECENIA
				 * Sprawdzanie możliwości wykonania ruchu i akcji
				/****************************************************************/
				case InputMode::character_is_selected:
				{
					// automatyczne spasowanie gdy zostało postaci 0 punktów akcji
					if (selectedCharacter->getAP() == 0)
					{
						finishTurn();
					}

					// akcje kliknięcia myszą dla elementów UI - LPM 
					// możliwe do wykonania tylko w trybie aktywnej tury gracza, gdy nie jest wywoływana inna akcja
					for (auto & b: ui.button)
					{
						if (mouseClicked(sf::Mouse::Left) && b->getGlobalBounds().contains(m_pos_on_window) && !MouseLClickedLastFrame)
						{
							if (b->getAction() == Button::attack)
							{
								// jeżeli liczba akcji jest wystarczająca by wykonać tę akcję, zaznacza przycisk
								if (selectedCharacter->getAP() >= b->getAbility()->getAP())
								{
									ui.selectButton(b.get());
								}
							}
							else
							{
								auto action_on_click =  ui.selectButton(b.get());
								// rozpoznano akcję zakończenia tury
								if (action_on_click == Button::endturn)
								{
									finishTurn();
								}	
							}
						}
					}
					// sprawdzenie akcji na planszy wymaga najechania myszką na jakieś pole
					if (hoveredField != nullptr)
					{
						// sprawdzenie odbywa się za tylko gdy zmieniło się wskazywane myszką pole (żeby nie odświeżac co klatkę)
						if (hovered_field_changed()) 
						{
							// tworzy na ekranie podgląd ruchu i/lub akcji do wykonania
							checkMoveAndActionsAuto();
						}

						/** Sprawdzenie czy jest jakaś akcja do wykonania (bądź samo przesunięcie)
						 *  Jeżeli klinięto na lewym przyciskiem myszy to zaakceptuj wywołanie
						 */
						if (	mouseClicked(sf::Mouse::Left, hoveredField->getGlobalBounds(), m_pos_on_map)
							&&	!MouseLClickedLastFrame
							&& (!road.empty() || !range.empty()) )
						{
							// zatwierdzenie akcji do wywołania
							acceptMoveAndAction();

							// zablokowanie gry aż do skończenia animacji
							lockGameMode();
						}
					} 
					else
					{
						// kursor znajduje się poza planszą - wyłączenie podglądu akcji do wywołania
						road.clear();
						range.clear();
					}

					// kliknięcie myszką w przycisk wywołania akcji
					if (ui.getSelectedBtn() )
					{
						createRangePreview(ui.getSelectedBtn()->getAbility()->get_in_range());
						range_created_from_auto = false;

						inputMode = action_is_selected;
					}
				} break;
				/****************************************************************
				 * Tryb inputu: 		WYRANO AKCJE, NALEŻY WYBRAĆ POLE WYWOŁANIA
				 * Sprawdzanie możliwości rozegrania akcji na dostępnym polu
				 * Zaznaczenie pola poza dostępnym obszarem anuluje akcję
				/****************************************************************/
				case InputMode::action_is_selected:
				{
					// jakiekolwiek kliknięcie LPM oznacza albo akceptacje albo anulowanie akcji
					if (	mouseClicked(sf::Mouse::Left)
						&&	!MouseLClickedLastFrame )
					{
						// sprawdzanie czy na wskazywanym polu jest postać na której można wywołać akcje							
						if (hoveredField && isEnemyOnHoveredField())
						{
							Attack attack = *(ui.getSelectedBtn()->getAbility());
							acceptAttack(attack);
						} 
						// nie ma wskazano prawidłowego celu - anulowanie wywoływania
						else
						{
							range.clear();
						}
						// zdezaktyowowanie klikniętego przycisku
						ui.unselectButtons();

						// powrót do zczytywania inputu dla zaznaczonej postaci bez wybranej żadnej akcji
						inputMode = InputMode::character_is_selected;
					}
				} break;
				}
				
				// pokazanie podglądu ataku na planszy gdy najedzie się myszką na przycisk
				if (ui.getHoveredBtn() && ui.getHoveredBtn()->getAction() == Button::attack )
				{
					createRangePreview(ui.getHoveredBtn()->getAbility()->get_in_range());
					range_created_from_auto = false;
				}
				
			} break;
			}

			// zapisanie ostatnio wkszywanego myszką pola
			lastHoveredField = hoveredField;
		}
		
		// ponowne zapisanie informacji czy kliknięto LPM myszki
		if (mouseClicked(sf::Mouse::Left))
			MouseLClickedLastFrame = true;
		else
			MouseLClickedLastFrame = false;

		// ******************************************************************
		// ******** Update wszystkich elemnetów w bieżącej klatce ************
		// ******************************************************************

		// zmierzenie czasu jaki upłynął pomiędzy klatkami
		float deltaTime = time.getElapsedTime().asSeconds() - lastUpdate.asSeconds();
		lastUpdate = time.getElapsedTime();

		// update dla wszystkich elementów gry
		update(deltaTime);

		// update przycisków z interfejsu użytkownika
		ui.updateButtons(m_pos_on_window, deltaTime);

		// odblokowanie możliwości ruchu po skończeniu wywołania kolejki animacji
		if (!anim_manager.anyAnimationLocking())
		{
			unlockGameMode();
		}

		// ******************************************************************
		// ********* Wyczyszczenie ekranu i narysowanie wszystkiego *********
		// ******************************************************************
		window->clear();
		
		// narysowanie elementów statycznych, stale widocznych na ekranie w tym samym miejscu
		window->setView(window->getDefaultView());
		draw_static_elements();

		// narysowanie scrollowanej widokiem kamery planszy gry
		window->setView(camera);
		draw_board();

		window->display();
	}
}

void Game::update(float delta)
{
	// update kolejki animacji
	anim_manager.updateAnimationsStack(delta);

	// update animacji bezczynych postaci
	anim_manager.updateIdleAnimations(delta);

	// update wszystich postaci
	for (auto &character : charactersOnBoard)
		character->update(delta);
		
	// animacja trójkącika nad zaznaczoną postacią
	if (selectedCharacter)
	{
		indicator_modifier += delta * (indicator_frame_raise ? 1 : -1);

		if (indicator_frame_raise)
			while (indicator_modifier > 1)
			{
				indicator_modifier = 1;
				indicator_frame_raise = false;
			}
		else
			while (indicator_modifier < 0)
			{
				indicator_modifier = 0;
				indicator_frame_raise = true;
			}

		indicator_selected_character.setScale(0.4 + indicator_modifier / 2.5, 1.0 - indicator_modifier / 4);
		indicator_selected_character.setPosition(selectedCharacter->getPosition() 
													+ sf::Vector2f{50, 50}
													+ sf::Vector2f{	indicator_selected_character.getGlobalBounds().width / 2,
																	indicator_modifier * 3.4f}
													+ sf::Vector2f{0, -48}
												);
	}

	// update kamery
	camera.update(delta);

	if (selectedCharacter)
	{
		// update wyświetlanych punktów akcji w okienku UI
		std::stringstream ss;
		ss << selectedCharacter->getAP() << "/" << selectedCharacter->getMaxAP();
		// wyświetlenie obrażeń zadawanych przez tą umiejętność
		ui.box_action_points.setString(ss.str());
	}
}

void Game::selectCharacter(CharacterOnBoard* character)
{
	inputMode = InputMode::character_is_selected;
	
	selectedCharacter = character;

	// zresetowanie ilości punktów akcji na maksymalną wartość
	selectedCharacter->setAP(selectedCharacter->getMaxAP());

	ui.textfieldSelectedCharacter.setString(selectedCharacter->getName());

	ui.cancelSimulatingHover();
	ui.destroyButtons();

	// dodanie przycisków z umiejętnościami (1. wiersz)
	for (float i = 0; i < selectedCharacter->get_attack_data().size(); i++)
		ui.addNewButton({i * 120, 0},
						selectedCharacter->get_attack_data()[i].button_data,
						&selectedCharacter->get_attack_data()[i].attack );

	// dodanie przysku zakończenia tury (2. wiersz, na końcu z prawej)
	ui.addNewButton({480 + 48, 122},
					selectedCharacter->get_finish_turn_button(),
					Button::Action::endturn );

	// zaznaczenie ataku pierwszego jako akcja domyslna
	ui.autoselectButton(ui.button[0].get());
}

void Game::checkMoveAndActionsAuto()
{
	// lista pól, przez które nie można przechodzić
	std::vector<Field*> blockedFields;

	// znalezienie pól gdzie stoi już inna postać
	for (auto &character : charactersOnBoard)
	{
		auto af = get_active_field_from_absolute_coords(character->getCoords());
		// pozycja początkowa postaci nie może być jako zablokowane pole
		if (character.get() != selectedCharacter)	
			blockedFields.push_back(af);
	}

	// utworzenie obiektu do znalezienia najkrótszej drogi podając informację o aktywnej części planszy
	Pathfinder pathfinder(activeField, blockedFields);
	
	// pole startowe drogi = pole zaznaczonej postaci
	Field* fieldA = get_active_field_from_absolute_coords(selectedCharacter->getCoords());

	// ustawienie ilości punktów akcji w podglądzie na początkową wartość, od której będzie odejmowane każde kolejne działanie
	AP_preview = selectedCharacter->getAP();

	// sprawdzenie czy są na najechanym polu są też akcje do wywołania
	checkActionsByHover();

	// w przypadku gdy nie ma w tej chwili podglądu żadnej akcji wyczyść podgląd drogi i utwórz od nowa
	if (range.empty()) 
	{
		// czyszczenie road za każdym razem gdy zmieniło się wskaztwane myszką pole
		road.clear();

		// próba utworzenie drogi: od pola postaci do wskazywanego pola
		if (hoveredField 	!= nullptr
			&& fieldA 		!= nullptr
			&& fieldA != hoveredField)
		{
			int max_movement = selectedCharacter->getAP();
			road = pathfinder.astar_search(fieldA, hoveredField, max_movement);
		}
	}

	// odjęcie punktów akcji w podglądzie za każdy obecny ruch do wykonania
	AP_preview -= road.get().size();

	updateAPpreviewOnBoard();
}

void Game::checkActionsByHover()
{
	// utworzenie kopii podglądu akcji na potrzeby sprawdzenia punktów akcji po odjęciu ewentualnego kosztu drogi do przejścia
	auto AP_preview_local = AP_preview;
	AP_preview_local -= road.get().size();

	if (!ui.getAutoselectedBtn())
		return;
		
	Attack attack = *(ui.getAutoselectedBtn()->getAbility());

	auto possible_range = attack.get_in_range();

	// pole z którego wywoływana zostaje akcja
	Field* field_caster;

	// dla ataków z bliska końcowe pole road stanowi pole wywołania (ruch postaci + akcja ataku)
	if (attack.get_type() == Attack::Type::melee)
	{
		if (road.empty())
			// w momencie gdy nie ma do wykonania ruchu, akcji wywoływana jest z obcenego pola
			field_caster = get_active_field_from_absolute_coords(selectedCharacter->getCoords());
		else
			field_caster  = road.getLastElement();
	}
	// dla ataków z zasięgu wywoływanie odbywa się zawsze z obecnego pola, akcja ruchu zostaje usunięta
	else if (attack.get_type() == Attack::Type::ranged)
	{
		field_caster = get_active_field_from_absolute_coords(selectedCharacter->getCoords());
		road.clear();
	}
	
	// szukane pole na którym można wywołać akcję
	Field* action_field {nullptr};

	// sprawdzenie czy pole na którym można wykonać akcję jest w jej zasięgu wywołania + stoi tam inna postać
	for(auto & r : possible_range) 
	{
		// uzyskanie współrzędnych kolejnych pól w zasięgu wywołania akcji
		auto coords_in_range = field_caster->getCoords() + r;

		for (auto &character : charactersOnBoard)
			// sprawdzenie czy na wskazywanym polu znajduje się inna postać
			if (	hoveredField->getCoords() == character->getCoords()
				&&	hoveredField->getCoords() == coords_in_range
				&&	character.get() != selectedCharacter)
				{
					// umieszczenie ataku w podglądzie jeżeli ilość akcji będzie wystarczająca by go wykonać
					if (AP_preview_local >= attack.getAP())
						action_field = get_active_field_from_absolute_coords(coords_in_range);
					
					// dalsze sprawdzanie nie jest potrzebne, bo tylko na jednym polu naraz można wywołać akcję
					break;
				}
	}

	std::vector<Field*> action_fields;

	if (action_field)
	{
		action_fields.push_back(action_field);

		// podświetlenie przycisku akcji, która może być teraz wywołana
		ui.simulateHover(ui.getAutoselectedBtn());

		// odjęcie punktów akcji w podglądzie ile zostanie po ataku
		AP_preview -= attack.getAP();
	} else
		ui.cancelSimulatingHover();

	// utworzenie podglądu zasięgu
	range = Range(	action_fields,
					get_active_field_from_absolute_coords(field_caster->getCoords() ) );

	range_created_from_auto = true;

}

void Game::updateAPpreviewOnBoard()
{
	if (hoveredField && selectedCharacter)
	{
		// update wyświetlanych punktów akcji na środku aktywnego pola planszy
		std::stringstream ss;
		ss << AP_preview << "/" << selectedCharacter->getMaxAP();
		text_AP_preview.setString(ss.str());
		text_AP_preview.setPosition(	hoveredField->getPosition()
										+ sf::Vector2f{	(tile_size - text_AP_preview.getGlobalBounds().width) / 2, 
														(tile_size - text_AP_preview.getGlobalBounds().height) / 2 - 3} 
									);
		text_AP_preview.setFillColor(sf::Color::Yellow);
	}
}

void Game::acceptMoveAndAction()
{
	if (!road.empty())
	{
		// wywołanie ruchu
		acceptMovePlayer();
	}
	if (!range.empty())
	{
		// pobiera informację o autoataku z przycisku z domyślnie wywoływanym atakiem
		Attack attack = *(ui.getAutoselectedBtn()->getAbility());

		// wywołanie ataku
		acceptAttack(attack);
	}
}

void Game::acceptMovePlayer()
{
	// zmiana położenia postaci od razu na docelową pozycję w kontekście logiki gry
	selectedCharacter->setCoords(road.getLastElement()->getCoords());

	// dodanie animacji przesuwającej postać pole po polu
	for (auto & r: road.get())
		anim_manager.addAnimationToQueue (
			anim_manager.createAnimationMove(
				selectedCharacter,
				Animations::Actions::Move{road.getOffsetToThisField(r), 240.0f}
			)
		);

	// odjęcie punktów akcji, po 1 za każde pokonane pole
	selectedCharacter->setAP(selectedCharacter->getAP() - road.get().size());

	// wyczyszczenie podglądu drogi
	road.clear();
}

void Game::acceptAttack(Attack& attack)
{
	auto r = hoveredField;

	CharacterOnBoard* attackedCharacter;

	// znalezienie postaci na atakowanym polu
	for( auto & character : charactersOnBoard) {
		if (character->getCoords() == r->getCoords())
			attackedCharacter = character.get();
	}

	// dodanie animacji ataku wykonywanego przez postać która atakuje
	// TODO zmienić na activity pobrane z ataku
	anim_manager.addAnimationToQueue (
		anim_manager.createAnimation(
			selectedCharacter,
			Character::Activity::attack1,
			range.getDirectionToThisField(r) 
		)
	);

	// odjęcie punktów akcji za atak
	selectedCharacter->setAP(selectedCharacter->getAP() - attack.getAP());

	// dodanie animacji otrzymania obrażeń zaatakowanej postaci
	anim_manager.addAnimationToQueue (
		anim_manager.createAnimationHurt(
			attackedCharacter,
			attack.draw_damage()
		)
	);

	// usunięcie podglądu wywołania akcji
	range.clear();	
}

void Game::createRangePreview(std::vector<sf::Vector2i> in_range)
{
	std::vector<Field*> vec;

	// znalezienie pól będacych w zasięgu
	for (auto & r : in_range )
	{
		int xx = selectedCharacter->getCoords().x + r.x;
		int yy = selectedCharacter->getCoords().y + r.y;

		// sprawdzenie czy pole istenieje na liście aktywnych pól planszy (jest na widocznym ekranie)
		auto af = get_active_field_from_absolute_coords({xx,yy});
		if (af != nullptr )
			vec.push_back(af);
	}

	range = Range(vec, get_active_field_from_absolute_coords(selectedCharacter->getCoords()));
}

void Game::finishTurn()
{
	battle_queue.switchToNextCharacter();
	selectCharacter(battle_queue.getCurrentCharacter());
}

bool Game::isEnemyOnHoveredField()
{
	for (auto & r : range.get())
		for (auto &character : charactersOnBoard)
			if (	hoveredField->getCoords() == character->getCoords()
				&&	r == hoveredField )
				return true;
	return false;
}

Field* Game::get_active_field_from_absolute_coords(sf::Vector2i coords)
{
	if (	coords.x-coordsTopLeft.x < 0
		||	coords.x-coordsTopLeft.x >= 8
		|| 	coords.y-coordsTopLeft.y < 0
		||	coords.y-coordsTopLeft.y >= 8
	)
		return nullptr;

	return activeField[coords.x-coordsTopLeft.x][coords.y-coordsTopLeft.y];
}

void Game::lockGameMode() 
{
	gameMode = locked_player_turn;
}

void Game::unlockGameMode() 
{
	gameMode = player_turn;
	justUnlocked = true;

	// żeby można było od razu wykryć akcję na wskazywanym polu (bez konieczności jego zmiany)
	lastHoveredField = nullptr;
}

bool Game::mouseClicked(sf::Mouse::Button mouse_btn, sf::FloatRect objRect, sf::Vector2f m_pos)
{
	if ((sf::Mouse::isButtonPressed(mouse_btn))
	 &&	(objRect.contains(m_pos)))
				return true;
	return false;
}

bool Game::mouseClicked(sf::Mouse::Button mouse_btn)
{
	if (sf::Mouse::isButtonPressed(mouse_btn))
		return true;
	return false;
} 

bool Game::hovered_field_changed()
{
	if (hoveredField != lastHoveredField)
		return true;

	return false;
}

void Game::draw_board()
{
	// narysowanie wszystkich widocznych pól planszy
	for (int x = 0; x < level.width; x++)
		for (int y = 0; y < level.height; y++)
		{
			auto f = *level.board[x][y];

			// funnkcja SFML zamieniająca współrzędne kamery na ekranowe
			sf::Vector2i pos =
				window->mapCoordsToPixel(f.getPosition());

			// ograniczenie wyświetlenia tylko do kafelków który pokrywają się z ekranem kamery
			if ( pos.x + tile_size > screen_size.x * camera.getViewport().left &&
				pos.y + tile_size > screen_size.y * camera.getViewport().top && 
				pos.x < screen_size.x * (camera.getViewport().left + camera.getViewport().width) &&
				pos.y < screen_size.y * (camera.getViewport().top + camera.getViewport().height) )
				window->draw(f);
		}

	// narysowanie wszystkich postaci
	for (auto &character : charactersOnBoard)
		window->draw(*character);
	
	if (selectedCharacter)
		window->draw(indicator_selected_character);

	// jeśli do wyświetelnia jest podgląd akcji nie podświetla się wskazywane pole
	bool hide_hovering = false;
	
	// narysowanie drogi przesunięcia postaci na pólach do przejścia
	if (!road.empty()) {
		window->draw(road);
	}

	if (!range.empty()) {
		window->draw(range);
	}

	if (!road.empty()  || (!range.empty() && range_created_from_auto))
	{
		window->draw(text_AP_preview);
	}

	// podświetelenie pola wskazywanego kursorem
	if (hoveredField)
		window->draw(hoveredRect);

}

void Game::draw_static_elements()
{
	window->draw(background);
	window->draw(borderForGameScreen);

	// narysowanie interfejsu
	window->draw(ui);
}