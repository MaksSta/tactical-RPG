#include "Button.h"

Button::Button( Attack* _ability,
                sf::Vector2f pos,
                std::string img_file_path,
                std::wstring _desc)
:
    ability{_ability}
{
    setPosition(pos);
    texture.loadFromFile(img_file_path);
    setTexture(texture);
    desc = _desc;
}

void Button::Update(float deltaTime)
{
    switch(state) {
        case inactive:
            setColor(sf::Color{255, 255, 255});
            break;
        case hovered:
            setColor(sf::Color{144, 144, 255, 240});
            break;
        case simulated_hovered:
            setColor(sf::Color{255, 144, 144, 240});
            break;
        case selected:
            setColor(sf::Color{255, 92, 92});
            break;
    }
}

Attack* Button::getAbility()
{
    return ability;
}

std::wstring Button::getDesc()
{
    return desc;
}