#include "Textfield.h"

Textfield::Textfield(sf::Vector2f pos, std::wstring text)
{
    setPosition(pos);
    
	// załadowanie czcionki
    font.loadFromFile("data/fonts/EncodeSansWide-Black.ttf");

    setFont(font);
    setString(text);
}