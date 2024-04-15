#include "Textfield.h"

Textfield::Textfield(sf::Vector2f pos, std::wstring text, unsigned int font_size, sf::Color color)
{
    setPosition(pos);
    
	// załadowanie czcionki
    font.loadFromFile("data/fonts/EncodeSansWide-Black.ttf");

    setFont(font);
    setCharacterSize(font_size);
    setString(text);
    setFillColor(color);
}