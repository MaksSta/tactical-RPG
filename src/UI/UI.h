/**
 * Interfejs użytkownika
 * obszar, który zaczyna się w miejscu o podanych współrzednych,
 * zatem układ odniesienia w UI zaczyna się od tego punktu
 *
 * podczas inicjalizacji tworzone są pola tekstowe na wyświetlane informacje
 * przyciski dodawać można z zewnątrz prodecurą addNewButton
 */

#ifndef UI_UI_H_
#define UI_UI_H_

#include <memory>
#include <sstream>

#include "Button.h"
#include "TextBox.h"
#include "../Fonts.h"
#include "../Global.h"

class UI : public sf::Drawable {
public:
  /**
   * @param starts_at pozycja interfejsu na ekranie czyli początek układu współrzędnych dla obiektów UI
   */
  UI(sf::Vector2f starts_at);

  /**
   * tworzy nowy przycisk wywołujący atak
   * @param pos pozycja względna od początku interfejsu użytkownika
   * @param data dane dotyczące przycisku
   * @param ability wskaźnik do umiejętności wywoływanej tym przyciskiem
   */
  void addNewButton(sf::Vector2f pos,
                    Button_data& data,
                    Abilities::Attack& ability);

  /**
   * tworzy nowy przycisk wywołujący specjalne zdarzenie
   * @param pos pozycja względna od początku interfejsu użytkownika
   * @param data dane dotyczące przycisku
   * @param _action specjalne zdarzenie, które jednoznacznie determinuje co robi przycisk po wciśnięciu
   */
  void addNewButton(sf::Vector2f pos,
                    Button_data& data,
                    Button::Action _action);

  /**
   * aktualizacja stanów wszystkich przycisków
   * @param m_pos pozycja myszy w momemncie kliknięcia
   * @param deltaTime czas jaki upłynął od ostatniej klatki
   */
  void updateButtons(sf::Vector2f m_pos,
                     float deltaTime);

  // wybranie przycisku (wciśnięcie go)
  void selectButton(Button* _selectedBtn);

  // zmiana domyślnie wykonaywanej akcji na tę pod przyciskiem
  void autoselectButton(Button* _selectedBtn);

  // zdjęcie zaznaczenia z przycisku
  void unselectButtons();

  // zwraca obecnie wksazywany myszką przycisk
  Button* getHoveredBtn();

  // zwraca obecnie zaznaczony przycisk
  Button* getSelectedBtn();

  // zwraca przycisk do akcji wywoływanej domyślnie
  Button* getAutoselectedBtn();

  // włączenie podglądu przycisku podobnie jakby był najechany myszką
  void simulateHover(Button* button);

  // anulowanie podglądu zasymulowania najechania przycisku
  void cancelSimulatingHover();

  // usuwa wszystkie przyciski
  void destroyButtons();

  // narysowanie całego interfejsu użytkownika na ekranie
  void draw(sf::RenderTarget &target,
            sf::RenderStates states) const;

  friend class Game;
private:
  // wszystkie przyciski będące interfejsem użytkownika
  std::vector<std::unique_ptr<Button>> button;

  // koszt akcji do wyświetlenia pod każdym przyciskiem
  std::vector<std::unique_ptr<Textfield>> text_action_cost;

  // pozycja na ekranie na której wyświetlony będzie pierwszy przycisk
  sf::Vector2f btnsStartPos;

  // przycisk traktowany jako aktywny np. najeżdzany myszą lub klinięty (bądź jego brak)
  Button* anyButtonActive;

  // przycisk pod którym obecnie znajduje się pod myszką
  Button* hoveredBtn{nullptr};

  Button* simulatedHoveredBtn{nullptr};

  // obecnie wybrany przycisk
  Button* selectedBtn{nullptr};

  // wstępnie wybrany przycisk (do akcji wywoływanej domyślnie)
  Button* autoselectedBtn{nullptr};

  std::map<std::string, std::unique_ptr<TextBox>> textBoxes;

  // otoczka rysowana wokół zaznaczonego przycisku
  sf::RectangleShape selectedBtnBorder;
};

#endif  /* UI_UI_H_ */
