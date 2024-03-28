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

// zestaw wszystkich dancych o pojedyńczym ataku
struct Attack_full_data {
	// informacje dotyczące samego ataku
	Attack attack;

	// aktywność, która informuje o zestawie animacji do tego ataku
	Character::Activity activity;

	// dane dotyczące przycisku wywołującego atak
	Button_data button_data;
};

class CharacterOnBoard : public sf::Drawable, public Character {
public:
	/** 
	 * Utworzonie postaci, którą po zainicjowaniu wyświetlać się będzie na ekranie
	 * \param _name nazwa postaci do wyświetlenia
	 * \param _max_hp maksymalna i zarazem początkowa ilość punktów życia
	 * \param start_coords pocztąwkowa pozycja na planszy
	 * \param _padding opcjonalny padding wyświetlanej postaci
	 * \param _scale opcjonalne skalowanie wielkości wyświetlanej postaci
	*/
	CharacterOnBoard(	std::string _name,
						short _max_hp,
						sf::Vector2i start_coords, 
						sf::Vector2f _padding = {0, 0},
						float _scale = 1.0 );

	/**
	 * ładowanie animacji dla danej aktywności i kierunku z jednego pliku png
	 * dzielonego na klatki umieszczone na "taśmie" wielkości img_w * frames
	 * \param activity czynność do której przypisana będzie animacja
	 * \param direction kierunek do którego przypisana będzie animacja
	 * \param src_img prowadzi jest do obrazka
	 * \param fps ile animować klatek / sekundę
	 * \param frames ilość klatek do załadowania
	 * \param img_w, img_h rozmiary jednej klatki
	*/
	void loadAnimation(	Activity activity,
	 					Direction direction,
						std::string	src_img,
						int frames,
						float fps,
						int img_w,
						int img_h );

	void update(float deltaTime);

	// inicjalizacja domyślnych ustawień dla postaci
	void init();

	// pobiera współrzędne postaci na planszy liczone w polach
	sf::Vector2i getCoords();

	// ustawia współrzędne postaci na planszy liczone w polach
	void setCoords(sf::Vector2i);

	// pobiera sprity dla obecnej aktywności i kierunku postaci
	Sprites_data& get_current_sprites_data();

	// przestawienie sprite na pierwszą klatkę z get_current_sprites_data();
	void reset_texture();

	// zmniejsza wartość hp postaci i dodaje animcję na pasku życia
	void takeDamage(int dmg);

	// zwraca pełne informacje o wszystkich atakach
	std::vector<Attack_full_data>& get_attack_data();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void move(sf::Vector2f offset);
	sf::FloatRect getGlobalBounds();
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f pos);
	void setTexture(sf::Texture&);
protected:
	// sprite z obrazkiem postaci
	sf::Sprite sprite;
	
	// pasek hp wyświetlany nad postacią
	HpBar hpBar;

	// wektor pełnych informacji do każdego ataku
	std::vector<Attack_full_data> attack_full_data;

	// współrzędne postaci na ekranie
	sf::Vector2f position;

	// współrzędne na planszy liczone w kafelkach
	sf::Vector2i coords;

	// odstęp od par boków lewo+prawo, góra+dół
	sf::Vector2f padding;

	// odwzorowanie {aktywność, kierunek} na przypisany zestaw spritów
	std::map<std::pair<Activity, Direction>, Sprites_data> sprites_data;

};

#endif /* CHARACTERS_CHARACTERONBOARD_H_ */