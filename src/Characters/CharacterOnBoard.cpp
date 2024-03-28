#include "CharacterOnBoard.h"

CharacterOnBoard::CharacterOnBoard(	std::string _name,
									short _max_hp,
									sf::Vector2i start_coords,
									sf::Vector2f _padding, 
									float _scale) 
: 
	Character	(_name, _max_hp),
	coords		{start_coords},
	padding		{_padding},
	hpBar		{getPosition(), hp, max_hp}
{	

	sprite.setScale(_scale, _scale);
}

void CharacterOnBoard::loadAnimation(	Activity activity,
										Direction direction,
										std::string src_img,
										int frames,
										float fps,
										int img_w,
										int img_h)
{
	Sprites_data newOne;
	newOne.fps = fps;

	// ładowanie tekstur z pliku
	for (int i = 0; i < frames; i++)
	{
		sf::Texture t;
		t.loadFromFile(src_img, (sf::IntRect(sf::Vector2i(img_w * i, 0), sf::Vector2i(img_w, img_h))));
		newOne.textures.push_back(t);
	}

	sprites_data[{activity, direction}] = newOne;
}

Sprites_data& CharacterOnBoard::get_current_sprites_data()
{
	return sprites_data[
		{currentActivity, direction}
	];	
}

void CharacterOnBoard::update(float deltaTime)
{
	// dodanie połowy paddingu
	sprite.setPosition(getPosition() - padding);

	hpBar.setPosition(getPosition());
	hpBar.update(deltaTime);
}

sf::Vector2i CharacterOnBoard::getCoords()
{
	return coords;
}

void CharacterOnBoard::setCoords(sf::Vector2i new_coords)
{
	coords = new_coords;
}

void CharacterOnBoard::reset_texture() {

	sprite.setTexture(get_current_sprites_data().textures[0]);
}

void CharacterOnBoard::takeDamage(int dmg)
{
	// ograniczonie otrzymanych obrażeń do pozostałej ilości hp
	int final_dmg = std::min(dmg, hp);
	hp -= final_dmg;
	
	// animacja na pasku hp
	hpBar.takeDamage(final_dmg);
}

std::vector<Attack_full_data>& CharacterOnBoard::get_attack_data()
{
	return attack_full_data;
}

void CharacterOnBoard::init()
{

	currentActivity = Character::idle;
	direction = down;

	// ustawienie tekstury na startową 
	reset_texture();
	
	// umiejscowienie postaci na ekranie
	setPosition(sf::Vector2f(getCoords().x * tile_size, getCoords().y * tile_size));
}


void CharacterOnBoard::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// wyświetlenie postaci
	target.draw(sprite);

	// pasek hp nad postacią
	target.draw(hpBar);
}


void CharacterOnBoard::move(sf::Vector2f offset) 
{
	position += offset;
}

sf::FloatRect CharacterOnBoard::getGlobalBounds() 
{
	return sprite.getGlobalBounds();
}


sf::Vector2f CharacterOnBoard::getPosition() 
{
	return position;
}

void CharacterOnBoard::setPosition(sf::Vector2f pos) 
{
	position = pos;
}

void CharacterOnBoard::setTexture(sf::Texture& texture)
{
	sprite.setTexture(texture);
}