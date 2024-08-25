/**
 * Graficzna implementacja klasy Character
 * Dodatkowo posiada:
 *  - współrzędne w polach na planszy
 *  - informacje o atakach
 *  - pasek z hp wyświetlany nad głową
 */

#ifndef CHARACTERS_CHARACTERONBOARD_H_
#define CHARACTERS_CHARACTERONBOARD_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

#include "Character.h"
#include "HpBar.h"
#include "../Abilities/Attack.h"
#include "../Global.h"

// zestaw wszystkich danych o pojedyńczej animacji sprita
struct Sprites_data {
  std::vector<sf::Texture> textures;
  float fps;
};

class CharacterOnBoard : public sf::Drawable, public Character {
public:
  /**
   * Utworzonie postaci, którą po zainicjowaniu wyświetlać się będzie na ekranie
   * @param start_coords pocztąwkowa pozycja na planszy
   * @param _name nazwa postaci do wyświetlenia
   * @param _team przez kogo sterowana jest postać
   * @param _max_hp maksymalna i zarazem początkowa ilość punktów życia
   * @param _padding opcjonalny padding wyświetlanej postaci
   * @param _scale opcjonalne skalowanie wielkości wyświetlanej postaci
   */
  CharacterOnBoard(sf::Vector2i start_coords,
                   std::string _name,
                   Team _team,
                   short _max_hp,
                   sf::Vector2f _padding = {0, 0},
                   float _scale = 1.0 );

  /**
   * ładowanie animacji dla danej aktywności i kierunku z jednego pliku png
   * dzielonego na klatki umieszczone na "taśmie" wielkości img_w * frames
   * @param activity czynność do której przypisana będzie animacja
   * @param direction kierunek do którego przypisana będzie animacja
   * @param src_img prowadzi jest do obrazka
   * @param fps ile animować klatek / sekundę
   * @param frames ilość klatek do załadowania
   * @param img_w, img_h rozmiary jednej klatki
   */
  void loadAnimation(Activity activity,
                     Direction direction,
                     std::string src_img,
                     int frames,
                     float fps,
                     int img_w,
                     int img_h );

  void update(float deltaTime);

  // inicjalizacja domyślnych ustawień dla postaci
  void init();

  sf::Vector2i getCoords() const;
  void setCoords(sf::Vector2i);

  // pobiera sprity dla obecnej aktywności i kierunku postaci
  // uwaga, ze względu na to że podaje wartość z mapy, ta metoda nie jest const
  Sprites_data& get_current_sprites_data();

  // przestawienie sprite na pierwszą klatkę dla obecnego kierunku i aktywności
  void reset_texture();

  // zmniejsza wartość hp postaci i dodaje animcję na pasku życia
  void takeDamage(int dmg);

  // zwraca pełne informacje o wszystkich atakach
  // uwaga, metoda zwraca odniesienie do orginalnego ataku więc nie jest const
  std::vector<Attack>& getAttacks();

  // uwaga, ze względu na to że podaje wartość z mapy, ta metoda nie jest const
  Button_data& getButtonData(Activity activity);

  // zwraca dane potrzebne do utworzenia przycisku zakończenia tury
  // uwaga, metoda zwraca orginalne dane przycisku więc nie jest const
  Button_data& get_finish_turn_button();

  // przestawia przezroczystość postaci wraz z paskiem hp
  void setTransparency(unsigned char transparency);

  // wygasza pokazywanie paska z hp
  void disableHpBar();

  void draw(sf::RenderTarget &target,
            sf::RenderStates states) const;

  void move(sf::Vector2f offset);
  sf::FloatRect getGlobalBounds() const;
  sf::Vector2f getPosition() const;
  void setPosition(sf::Vector2f pos);
  void setTexture(sf::Texture&);
protected:
  // przycisk zakończenia tury
  Button_data finish_turn_button;

  // sprite z obrazkiem postaci
  sf::Sprite sprite;

  // pasek hp wyświetlany nad postacią
  HpBar hpBar;

  // wektor wszystkich ataków postaci
  std::vector<Attack> attack;

  // współrzędne postaci na ekranie
  sf::Vector2f position;

  // współrzędne na planszy liczone w kafelkach
  sf::Vector2i coords;

  // odstęp od par boków lewo+prawo, góra+dół
  sf::Vector2f padding;

  // odwzorowanie {aktywność, kierunek} na przypisany zestaw spritów
  std::map<std::pair<Activity, Direction>, Sprites_data> sprites_data;

  // odwzorowanie ataku (podanego jako aktywność) na przycisk do niego
  std::map<Activity, Button_data> button_data_for_attack;

  bool displayHpBar{true};
};

#endif /* CHARACTERS_CHARACTERONBOARD_H_ */
