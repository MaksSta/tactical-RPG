#include "Game.h"

Game::Game(sf::RenderWindow &_window,
           sf::Vector2u _screen_size,
           sf::Vector2f board_starts_at,
           sf::Vector2f board_screen_size)
  try:
  window{&_window},
  screen_size{_screen_size},
  camera{
    board_screen_size,
    sf::FloatRect{
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

  // ustawienie wielkości całej mapy
  camera.set_map_size(sf::Vector2f(tile_size * fullBoard.getWidth(),
                                   tile_size * fullBoard.getHeight()));

  activeBoard.loadActiveFields(coordsTopLeft, fullBoard);

  // utworzenie postaci
  charactersOnBoard.push_back(std::make_unique<Warrior>(sf::Vector2i{1, 1}));
  charactersOnBoard.push_back(std::make_unique<Sorceress>(sf::Vector2i{6, 1}));

  charactersOnBoard.push_back(std::make_unique<GoblinSlinger>(sf::Vector2i{2, 6}));
  charactersOnBoard.push_back(std::make_unique<GoblinBerserker>(sf::Vector2i{1, 7}));
  charactersOnBoard.push_back(std::make_unique<GoblinSlinger>(sf::Vector2i{5, 6}));
  charactersOnBoard.push_back(std::make_unique<GoblinBerserker>(sf::Vector2i{6, 7}));

  // TODO napisać do tego osobną funkcję do uruchamiania przy przewijaniu planszy
  // dodanie widocznych na ekranie postaci do aktywnej planszy
  for (auto &character : charactersOnBoard) {
    activeBoard.addCharacter(character.get());
  }

  // dodanie postaci do kolejki bitwy
  for (auto &character : charactersOnBoard) {
    battle_queue.addToQueue(character.get());
  }

  // ustawienie tury na pierwszą postać z kolejki
  battle_queue.setOnFirstCharacter();
  selectPlayerCharacter(battle_queue.getCurrentCharacter());

  // dodanie animacji bezczynności (idle) do każdej postaci
  for (auto &character : charactersOnBoard) {
    anim_manager.addIdleAnimation(anim_manager.createAnimation(character.get(),
                                                               Character::Activity::idle));
  }

  // graficzne ustawienia dla znacznika zaznaczonej postaci
  indicator_selected_character.setFillColor(sf::Color{0, 135, 0});
  indicator_selected_character.setRotation(180);

  // przygotowanie napisu Defeat
  text_defeat.setFont(font_rpg);
  text_defeat.setString("Defeat");
  text_defeat.setCharacterSize(150);
  text_defeat.setStyle(sf::Text::Bold);
  text_defeat.setFillColor(sf::Color{145, 0, 0, 0});
  text_defeat.setOrigin(text_defeat.getLocalBounds().getPosition());
  text_defeat.setPosition((window->getSize().x - text_defeat.getGlobalBounds().width) / 2,
                          (window->getSize().y - text_defeat.getGlobalBounds().height) / 2);
}
catch (errors::cannot_open_file &err_file)
{
  std::cout << "Error! Cannot open file: " << err_file.filename << std::endl;
  std::exit(1);
}

void Game::run()
{
  // domyślne rozpoczęcie gry z turą gracza
  gameMode = player_turn;
  inputMode = character_is_selected;

  // tu zapisywany będzie czas narysowania ostatniej klatki ekranu
  sf::Time lastUpdate = time.getElapsedTime();

  // początkowe ustawienie kamery
  camera.setCenter(8 * tile_size / 2 * 1,
                   8 * tile_size / 2 * 1);
  camera.setDstPos(camera.getCenter());

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
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++)
      {
        auto f = activeBoard.getField({x, y});
        if (f->getGlobalBounds().contains(m_pos_on_map))
          hoveredField = f;
      }
    }
    // jeśli znaleziono, zostanie ono podświetlone
    if (hoveredField)
    {
      hoveredRect.setFillColor(sf::Color{0, 20, 255, 16});
      hoveredRect.setSize(sf::Vector2f(tile_size, tile_size));
      hoveredRect.setPosition(hoveredField->getPosition());
    }

    // ******************************************************************
    // **************** Obsługa wszystkich wydarzeń *********************
    // ******************************************************************
    while (window->pollEvent(event) || waiting_for_action)
    {
      waiting_for_action = false;

      // systemowe zamknięcie aplikacji (np. kliknięcie X na pasku okna / alt+f4)
      if (event.type == sf::Event::Closed)
        window->close();

      // akcje kliknięcia myszą dla elementów UI - PPM
      // [możliwe do kliknięcia w każdej fazie gry]
      for (auto &b : ui.button)
      {
        if (mouseClicked(sf::Mouse::Right) && b->getGlobalBounds().contains(m_pos_on_window))
        {
          // pobranie umiejętności pod przyciskiem
          auto ability = b.get()->getAbility();
          if (ability != nullptr && ability->getCallType() == Abilities::Attack::CallType::targetable)
          {
            ui.autoselectButton(b.get());

            // zapisanie informacji z ostatnio wybranym atakiem atakiem domyślnym u tej postaci
            lastDefaultAttack[selectedCharacter] = b.get()->getAbility();
          }
        }
      }

      if (!gameLocked)
      {
        switch (gameMode)
        {
        case GameMode::no_battle:
        {
          /**
           * Tryb gry: PORUSZANIE SIĘ PO PLANSZY
           */
        } break;
        case GameMode::player_turn:
        {
          /**
           * Tryb gry: TURA GRACZA
           */

          switch (inputMode)
          {
          case InputMode::character_is_selected:
          {
            /**
             * Tryb inputu: ZAZNACZONA POSTAĆ, CZEKA NA POLECENIA
             * Sprawdzanie możliwości wykonania ruchu i akcji
             */
            bool turn_finished = false;
            
            // automatyczne spasowanie gdy zostało postaci 0 punktów akcji
            if (selectedCharacter->getAP() == 0) {
              finishTurn();
              turn_finished = true;
            }

            // akcje kliknięcia myszą dla elementów UI - LPM
            for (auto &b : ui.button)
            {
              if (mouseClicked(sf::Mouse::Left) && b->getGlobalBounds().contains(m_pos_on_window) && !MouseLClickedLastFrame)
              {
                if (b->getActivationType() == Button::ActivationType::selectable)
                {
                  if (b->getAction() == Button::Action::attack)
                  {
                    // jeżeli liczba akcji jest wystarczająca by wykonać tę akcję, zaznacza przycisk
                    if (selectedCharacter->getAP() >= b->getAbility()->getAP())
                    {
                      ui.selectButton(b.get());
                    }
                  }
                }
                else if (b->getActivationType() == Button::ActivationType::clickable)
                {
                  if (b->getAction() == Button::Action::attack)
                  {
                    // jeżeli liczba akcji jest wystarczająca, wywołuje natychmiastowo akcję
                    if (selectedCharacter->getAP() >= b->getAbility()->getAP())
                    {
                      attackAOE(*b->getAbility());
                      
                      lockGameMode();
                    }
                  }
                  else if (b->getAction() == Button::Action::endturn)
                  {
                    finishTurn();
                    turn_finished = true;
                  }
                }
              }
            }

            if (turn_finished)
              break;
            
            // sprawdzenie akcji na planszy wymaga najechania myszką na jakieś pole
            if (hoveredField != nullptr)
            {
              // sprawdzenie odbywa się za tylko gdy zmieniło się wskazywane myszką pole (żeby nie odświeżac co klatkę)
              if (hovered_field_changed())
              {
                // tworzy na ekranie podgląd ruchu i/lub akcji do wykonania
                checkMoveAndActionsAuto();
              }

              /**
               *  Sprawdzenie czy jest jakaś akcja do wykonania (bądź samo przesunięcie)
               *  Jeżeli klinięto na lewym przyciskiem myszy to zaakceptuj wywołanie
               */
              if (mouseClicked(sf::Mouse::Left, hoveredField->getGlobalBounds(), m_pos_on_map)
                  && !MouseLClickedLastFrame
                  && (!road.empty() || !range_player.empty()))
              {
                // zatwierdzenie akcji do wywołania
                acceptMoveAndAction();
                
                lockGameMode();
              }
            }
            else
            {
              // kursor znajduje się poza planszą - wyłączenie podglądu akcji do wywołania
              road.clear();
              range_player.clear();
              ui.cancelSimulatingHover();
            }

            // pokazanie podglądu ataku na planszy gdy najedzie się myszką na przycisk
            if (ui.getHoveredBtn() && ui.getHoveredBtn()->getAction() == Button::attack)
            {
              range_player = activeBoard.createRange(selectedCharacter, ui.getHoveredBtn()->getAbility()->get_in_range());
              range_created_from_auto = false;
            }

            // kliknięcie myszką w przycisk wywołania akcji
            if (ui.getSelectedBtn())
            {
              range_player = activeBoard.createRange(selectedCharacter, ui.getSelectedBtn()->getAbility()->get_in_range());
              range_created_from_auto = false;
              
              inputMode = action_is_selected;
            }
          } break;
          case InputMode::action_is_selected: {
            /**
             * Tryb inputu: WYRANO AKCJE, NALEŻY WYBRAĆ POLE WYWOŁANIA
             * Sprawdzanie możliwości rozegrania akcji na dostępnym polu
             * Zaznaczenie pola poza dostępnym obszarem anuluje akcję
             */

            // jakiekolwiek kliknięcie LPM oznacza albo akceptacje albo anulowanie akcji
            if (mouseClicked(sf::Mouse::Left) && !MouseLClickedLastFrame) {
              // sprawdzanie czy na wskazywanym polu jest postać na której można wywołać akcje i czy to pole jest w zasięgu ataku
              if (hoveredField && getEnemyOnHoveredField()
                  && activeBoard.isFieldInRange(hoveredField, range_player)
                  && getEnemyOnHoveredField()->getTeam() != selectedCharacter->getTeam()) {
                Abilities::Attack attack = *(ui.getSelectedBtn()->getAbility());
                
                acceptAttack(attack, getEnemyOnHoveredField(), range_player.getDirectionToThisField(hoveredField));
                
                lockGameMode();
              }
              else {
                // nie ma wskazano prawidłowego celu - anulowanie wywoływania
                range_player.clear();
              }
              
              // zdezaktyowowanie klikniętego przycisku
              ui.unselectButtons();
              
              // powrót do zczytywania inputu dla zaznaczonej postaci bez wybranej żadnej akcji
              inputMode = InputMode::character_is_selected;
            }
          } break;
          }
        } break;
        case enemy_turn: {
          /**
           * Tryb gry: TURA PRZECIWNIKA/KOMPUTERA
           */

          // wykrywanie końca walki - porażki w przypadku braku żywych graczy
          bool any_alive_players_characters{false};
          for (auto & character : charactersOnBoard)
          {
            if (character->isAlive() && character->getTeam() == Character::Team::player)
            {
              any_alive_players_characters = true;
              break;
            }
          }
          if (!any_alive_players_characters)
          {
            gameMode = GameMode::defeat;
            break;
          }

          // wygenerowanie najlepszej decyzji jaką AI widzi w danej chwili
          if (ai_decision.empty())
          {
            AI::Core ai(selectedCharacter, activeBoard);
            ai_decision = ai.calculateBestDecision();
          }

          // pobieranie kolejnych akcji do wywołania
          AI::Action* action = ai_decision.popNextAction();

          if (dynamic_cast<AI::Move*>(action))
          {
            moveCharacter(selectedCharacter, action->getRoad());

            lockGameMode();
            break;
          }

          if (dynamic_cast<AI::Attack*>(action))
          {
            acceptAttack(*(action->getAttackInfo().attack), action->getAttackInfo().target,
                         action->getAttackInfo().range.getDirectionToThisField(activeBoard.getFieldOccupedBy(action->getAttackInfo().target)));

            lockGameMode();
            break;
          }

          if (dynamic_cast<AI::FinishTurn *>(action))
          {
            finishTurn();
            break;
          }

        } break;
        case GameMode::defeat:
        {
          /**
           * Tryb gry: PORAŻKA
           */

        } break;
        }
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
    // ******** Update wszystkich elemnetów w bieżącej klatce ***********
    // ******************************************************************

    // zmierzenie czasu jaki upłynął pomiędzy klatkami
    float deltaTime = time.getElapsedTime().asSeconds() - lastUpdate.asSeconds();
    lastUpdate = time.getElapsedTime();

    // update dla wszystkich elementów gry
    update(deltaTime);

    // update przycisków z interfejsu użytkownika
    ui.updateButtons(m_pos_on_window, deltaTime);

    // odblokowanie interakcji w grze, gdy nie ma żadnej oczekującej animacji, która blokuje postaci
    if (!anim_manager.anyAnimationLocking())
    {
      unlockGameMode();
    }

    // żeby można było od razu wykryć akcję na wskazywanym polu (bez konieczności jego zmiany)
    lastHoveredField = nullptr;

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

    // TODO przeniść jako 3cia metoda: rysowanie na wierzchu
    window->setView(window->getDefaultView());
    window->draw(text_defeat);

    window->display();
  }
}

void Game::update(float delta)
{
  // uśmiercenie postaci dla których liczba hp spadła do zera
  for (auto &character : activeBoard.getAliveCharacters())
  {
    if (character->getHP() == 0 && !character->will_die_this_turn)
    {
      // zaznaczenie że postać zaraz umrze, by nie sprawdzać tego ponownie
      character->will_die_this_turn = true;

      // czas ostatniej animacji na tej konkretnej postaci, żeby śmierć była od razu po
      auto time = anim_manager.calculateTimeToLastAnimationFinish(character);
      std::cout << "DEATH --- czas do rozpoczęcia: " << time << "\n";


      Animations::Sequence & sequence_after_other_animations_finish =
        anim_manager.addNewSequenceParallelAfterTime(time);

      // dodanie animacji śmierci i następnie zniknięcia
      sequence_after_other_animations_finish.push_back(anim_manager.createAnimationDeath(character));
      sequence_after_other_animations_finish.push_back(anim_manager.createAnimationDisappear(character));
    }
  }

  // udpate animacji bezczynności
  anim_manager.updateIdleAnimations(activeBoard.getAliveCharacters(), delta);
  anim_manager.updateAnimationsStack(delta);

  // update dla wszystich postaci
  for (auto &character : charactersOnBoard) {
    character->update(delta);
  }

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
    indicator_selected_character.setPosition(selectedCharacter->getPosition() + sf::Vector2f{50, 2}
                                             + sf::Vector2f{indicator_selected_character.getGlobalBounds().width / 2,
                                                 indicator_modifier * 3.4f});
  }

  // update kamery
  camera.update(delta);

  if (selectedCharacter) {
    // update wyświetlanych punktów akcji w okienku UI
    std::stringstream ss;
    ss << selectedCharacter->getAP() << "/" << selectedCharacter->getMaxAP();
    // wyświetlenie obrażeń zadawanych przez tą umiejętność
    ui.box_action_points.setString(ss.str());
  }

  // napis PORAŻKA
  if (gameMode == GameMode::defeat && !anim_manager.anyAnimationLeft())
  {
    defeat_transparency += delta * 95;
    if (defeat_transparency> 255)
      defeat_transparency = 255;
    text_defeat.setFillColor(sf::Color{255,0,0, static_cast<unsigned char>(defeat_transparency)});
  }
}

void Game::selectPlayerCharacter(CharacterOnBoard* character)
{
  gameMode = player_turn;

  inputMode = InputMode::character_is_selected;

  selectedCharacter = character;

  // zresetowanie ilości punktów akcji na maksymalną wartość
  selectedCharacter->setAP(selectedCharacter->getMaxAP());

  ui.textfieldSelectedCharacter.setString(selectedCharacter->getName());

  ui.cancelSimulatingHover();
  ui.destroyButtons();

  // dodanie przycisków z umiejętnościami (1. wiersz)
  float n = 0;
  for (auto &attack : selectedCharacter->getAttacks()) {
    ui.addNewButton({120 * n, 0},
                    selectedCharacter->getButtonData(attack.getActivity()),
                    attack);
    n++;
  }

  // dodanie przycisku zakończenia tury (2. wiersz, na końcu z prawej)
  ui.addNewButton({480 + 48, 122},
                  selectedCharacter->get_finish_turn_button(),
                  Button::Action::endturn);

  // zaznaczenie domyślnie wykonywanej akcji na taką samą co ostatnio u tej postaci
  if (lastDefaultAttack[selectedCharacter]) {
    for (auto &b : ui.button)
      if (b->getAbility() == lastDefaultAttack[selectedCharacter]) {
        ui.autoselectButton(b.get());
        break;
      }
  } else {
    // jeżeli nie było zmiany domyślnego ataku, to jest nim pierwszy przycisk
    ui.autoselectButton(ui.button[0].get());
  }
}

void Game::selectEnemyCharacter(CharacterOnBoard *character)
{
  gameMode = enemy_turn;

  selectedCharacter = character;

  // zresetowanie ilości punktów akcji na maksymalną wartość
  selectedCharacter->setAP(selectedCharacter->getMaxAP());

  ui.textfieldSelectedCharacter.setString(selectedCharacter->getName());

  ui.cancelSimulatingHover();
  ui.destroyButtons();
}

void Game::checkMoveAndActionsAuto()
{
  // pobranie listy zablokowanych pól z pominięciem pola na którym stoi wybrana postać (by algorytm szukania drogi zadziałał)
  auto blockedFields = activeBoard.getBlockedFields();
  auto it = std::find(blockedFields.begin(), blockedFields.end(),
                      activeBoard.getFieldOccupedBy(selectedCharacter)); 
  blockedFields.erase(it);

  // utworzenie obiektu do znalezienia najkrótszej drogi podając informację o aktywnej części planszy
  Pathfinder pathfinder(activeBoard.field, blockedFields);

  // pole startowe drogi = pole zaznaczonej postaci
  Field* fieldA = activeBoard.getFieldOccupedBy(selectedCharacter);

  // ustawienie ilości punktów akcji w podglądzie na początkową wartość, od której będzie odejmowane każde kolejne działanie
  AP_preview = selectedCharacter->getAP();

  // sprawdzenie czy są na najechanym polu są też akcje do wywołania
  checkActionsByHover();

  // NOTE sprawdzić czy własnie tu znajduje się problem, że dla drugiego potencjalnego ataku skasuje drogę (miałem taki bug)

  // w przypadku gdy nie ma w tej chwili podglądu żadnej akcji wyczyść podgląd drogi i utwórz od nowa
  if (range_player.empty())
  {
    // czyszczenie road za każdym razem gdy zmieniło się wskazywane myszką pole
    road.clear();

    // próba utworzenie drogi: od pola postaci do wskazywanego pola
    if (hoveredField != nullptr && fieldA != nullptr && fieldA != hoveredField)
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

  Abilities::Attack attack = *(ui.getAutoselectedBtn()->getAbility());

  auto possible_range = attack.get_in_range();

  // pole z którego wywoływana zostaje akcja
  Field* field_caster;

  // dla ataków z bliska końcowe pole road stanowi pole wywołania (ruch postaci + akcja ataku)
  if (attack.get_type() == Abilities::Attack::Type::melee)
  {
    if (road.empty())
      // w momencie gdy nie ma do wykonania ruchu, akcji wywoływana jest z obecnego pola
      field_caster = activeBoard.getFieldOccupedBy(selectedCharacter);
    else
      field_caster = road.getLastElement();
  }
  // dla ataków z zasięgu wywoływanie odbywa się zawsze z obecnego pola
  else if (attack.get_type() == Abilities::Attack::Type::ranged)
  {
    field_caster = activeBoard.getFieldOccupedBy(selectedCharacter);

    // w przypadku znalezienia calu dla ataku dystansowego nie ma przesunięcia postaci
    road.clear();
  }
  
  // szukane pole na którym można wywołać akcję
  Field* action_field{nullptr};

  // sprawdzenie czy pole na którym można wykonać akcję jest w jej zasięgu wywołania + stoi tam inna postać
  for (auto &r : possible_range)
  {
    // uzyskanie współrzędnych kolejnych pól w zasięgu wywołania akcji
    auto coords_in_range = activeBoard.getCoordsOf(field_caster) + r;
    
    for (auto &character : activeBoard.getAliveCharacters())
    {
      // sprawdzenie czy na wskazywanym polu znajduje się wroga postać
      if (hoveredField == activeBoard.getFieldOccupedBy(character) &&
          activeBoard.getCoordsOf(hoveredField) == coords_in_range &&
          getEnemyOnHoveredField()->getTeam() != selectedCharacter->getTeam())
      {
        // umieszczenie ataku w podglądzie jeżeli ilość akcji będzie wystarczająca by go wykonać
        if (AP_preview_local >= attack.getAP())
          action_field = activeBoard.getField(coords_in_range);
        
        // dalsze sprawdzanie nie jest potrzebne, bo tylko na jednym polu naraz można wywołać akcję
        break;
      }
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
  }
  else
  {
    ui.cancelSimulatingHover();
  }

  // utworzenie podglądu zasięgu ataku
  range_player = Range(action_fields, field_caster);

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
    text_AP_preview.setPosition(hoveredField->getPosition() + sf::Vector2f{(tile_size - text_AP_preview.getGlobalBounds().width) / 2,
                                                                           (tile_size - text_AP_preview.getGlobalBounds().height) / 2 - 3});
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
  if (!range_player.empty())
  {
    // pobiera informację o autoataku z przycisku z domyślnie wywoływanym atakiem
    Abilities::Attack attack = *(ui.getAutoselectedBtn()->getAbility());

    // wywołanie ataku
    acceptAttack(attack, getEnemyOnHoveredField(), range_player.getDirectionToThisField(hoveredField));
  }
}

void Game::moveCharacter(CharacterOnBoard* character,
                         Road& road)
{
  // zmiana położenia postaci w logice gry od razu na docelową pozycję
  character->setLocalCoords(activeBoard.getCoordsOf(road.getLastElement()));
  character->setGlobalCoords(activeBoard.getCoordsOf(road.getLastElement()) + coordsTopLeft);

  auto& sequence_moves = anim_manager.addNewSetParallel();

  // dodanie animacji przesuwającej postać pole po polu
  for (auto &r : road.get())
  {

    // taki feature, że gdy już rozpoczyna się przesunięcie na docelowe pole to odblokowauje od razu grę
    // NOTE koncepcyjnie to na ten moment nie wypali, może poza sytuacją gdzie ostatni ruch natychmiast kończy turę
    // if (r == road.getLastElement())
    //   isBlocking = false;

    bool isBlocking = true;
    sequence_moves.push_back(anim_manager.createAnimationMove(
                               character,
                               Animations::Actions::Move{road.getOffsetToThisField(r),
                                                         240.0f},
                               isBlocking));
  }

  // odjęcie punktów akcji, po 1 za każde pokonane pole
  character->setAP(character->getAP() - road.get().size());
}

void Game::acceptMovePlayer()
{
  moveCharacter(selectedCharacter, road);
  
  // wyczyszczenie podglądu drogi
  road.clear();
}

void Game::acceptAttack(Abilities::Attack& attack,
                        CharacterOnBoard* target,
                        Direction attack_direction)
{
  // odjęcie punktów akcji za atak
  selectedCharacter->setAP(selectedCharacter->getAP() - attack.getAP());

  int drawn_damage = attack.draw_damage();
  int damage = std::min(drawn_damage, target->getHP());

  target->takeDamage(damage);

  // dodanie animacji ataku wykonywanego przez postać która atakuje
  Animations::Sequence & sequence_after_other_animations_finish_0 =
    anim_manager.addNewSequenceParallelAfterTime(anim_manager.calculateTimeToLastAnimationFinish());

  sequence_after_other_animations_finish_0.push_back(anim_manager.createAnimation(selectedCharacter,
                                                                                  attack.getActivity(),
                                                                                  attack_direction));

  // dodanie animacji otrzymania obrażeń zaatakowanej postaci
  auto time = anim_manager.calculateTimeToLastAnimationFinish();
  std::cout << "HURT --- czas do rozpoczęcia: " << time << " ms\n";

  Animations::Sequence & sequence_after_other_animations_finish_1 =
    anim_manager.addNewSequenceParallelAfterTime(time);
  Animations::Sequence & sequence_after_other_animations_finish_2 =
    anim_manager.addNewSequenceParallelAfterTime(time);

  // równolegle wywołanie Hurt z TakeDamageHP
  sequence_after_other_animations_finish_1.push_back(anim_manager.createAnimationHurt(target));

  sequence_after_other_animations_finish_2.push_back(anim_manager.createAnimationTakeDamageHP(target,
                                                                                              damage));

  // usunięcie podglądu wywołania akcji
  range_player.clear();
}

void Game::acceptMultiAttack(Abilities::Attack& attack,
                             std::vector<CharacterOnBoard*> targets,
                             Direction attack_direction)
{
  // odjęcie punktów akcji za atak
  selectedCharacter->setAP(selectedCharacter->getAP() - attack.getAP());

  // dodanie animacji ataku wykonywanego przez postać która atakuje
  Animations::Sequence & sequence_after_other_animations_finish_0 =
    anim_manager.addNewSequenceParallelAfterTime(anim_manager.calculateTimeToLastAnimationFinish());

  sequence_after_other_animations_finish_0.push_back(anim_manager.createAnimation(selectedCharacter,
                                                                                  attack.getActivity(),
                                                                                  attack_direction));
  auto time = anim_manager.calculateTimeToLastAnimationFinish();

  // otrzymanie obrażęń oraz dodanie animacji dla wszystkich zaatakowanych postaci równocześnie
  for (auto &target : targets)
  {
    int drawn_damage = attack.draw_damage();
    int damage = std::min(drawn_damage, target->getHP());


    Animations::Sequence & sequence_after_other_animations_finish_1 =
      anim_manager.addNewSequenceParallelAfterTime(time);
    Animations::Sequence & sequence_after_other_animations_finish_2 =
      anim_manager.addNewSequenceParallelAfterTime(time);

    sequence_after_other_animations_finish_1.push_back(anim_manager.createAnimationHurt(target));
    sequence_after_other_animations_finish_2.push_back(anim_manager.createAnimationTakeDamageHP(target, damage));

    target->takeDamage(damage);
  }

  // usunięcie podglądu wywołania akcji
  range_player.clear();
}

void Game::attackAOE(Abilities::Attack& attack)
{
  std::vector<CharacterOnBoard*> targets;

  // zlokalizuj wszystkie postacie w zasięgu ataku i dodaj do listy celów
  for (auto &range : range_player.get()) {
    if (activeBoard.getCharacterOnField(range))
      targets.push_back(activeBoard.getCharacterOnField(range));
  }

  acceptMultiAttack(attack, targets, selectedCharacter->getDirection());
}

void Game::finishTurn()
{
  battle_queue.switchToNextCharacter();

  auto currentCharacter = battle_queue.getCurrentCharacter();

  if (currentCharacter->getTeam() == Character::Team::player)
    selectPlayerCharacter(currentCharacter);
  else if (currentCharacter->getTeam() == Character::Team::enemy)
    selectEnemyCharacter(currentCharacter);

}


CharacterOnBoard* Game::getEnemyOnHoveredField() const
{
  return activeBoard.getCharacterOnField(hoveredField);
}

void Game::lockGameMode()
{
  gameLocked = true;
}

void Game::unlockGameMode()
{
  gameLocked = false;
  waiting_for_action = true;
}

bool Game::mouseClicked(sf::Mouse::Button mouse_btn,
                        sf::FloatRect objRect,
                        sf::Vector2f m_pos)
{
  if ((sf::Mouse::isButtonPressed(mouse_btn)) && (objRect.contains(m_pos)))
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
  for (int x = 0; x < fullBoard.getWidth(); x++) {
    for (int y = 0; y < fullBoard.getHeight(); y++) {
      auto f = *fullBoard.getField(x, y);

      // funkcja SFML zamieniająca współrzędne kamery na ekranowe
      sf::Vector2i pos = window->mapCoordsToPixel(f.getPosition());

      // ograniczenie wyświetlenia tylko do kafelków który pokrywają się z
      // ekranem kamery
      if (pos.x + tile_size > screen_size.x * camera.getViewport().left &&
          pos.y + tile_size > screen_size.y * camera.getViewport().top &&
          pos.x < screen_size.x * (camera.getViewport().left +
                                   camera.getViewport().width) &&
          pos.y < screen_size.y *
                      (camera.getViewport().top + camera.getViewport().height))
      {
        window->draw(f);
      }
    }
  }

  // narysowanie wszystkich postaci
  for (auto &character : charactersOnBoard) {
    window->draw(*character);
  }

  if (selectedCharacter)
    window->draw(indicator_selected_character);
  
  // jeśli do wyświetelnia jest podgląd akcji nie podświetla się wskazywane pole
  bool hide_hovering = false;
  
  // narysowanie drogi przesunięcia postaci na pólach do przejścia
  if (!road.empty()) {
    window->draw(road);
  }
  
  if (!range_player.empty()) {
    window->draw(range_player);
  }
  
  if (!road.empty() || (!range_player.empty() && range_created_from_auto)) {
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
