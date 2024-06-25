/**
 * Tekst do wyświetlanie na ekranie razem z tłem
 * ustawienia tekstu w poziomie w zalęzności od wybranego rodzaju align
*/

#ifndef UI_TEXTBOX_H_
#define UI_TEXTBOX_H_

#include "Textfield.h"

class TextBox : public sf::Drawable {
public:
    // rodzaje wyrównania tekstu
    enum Alignment {
        start,
        center,
        end
    };

    /**
     * Utworzenie pola tekstowego z tłem
     * \param pos pozycja na ekranie
     * \param size rozmiar tła
     * \param _alignment_horizontal sposób wyrównania tekstu w poziomie
    */
    TextBox(	sf::Vector2f pos,
				sf::Vector2f size,
				Alignment _alignment_horizontal,
                std::string label = "",
                std::string text = "",
                sf::Color color = sf::Color::White,
                unsigned int font_size = 24
            );

    // zmiana zawartości tekstu na string i wynikające z tego odswieżenie pozycji
    void setString(std::string text);

    // wyliczenie pozycji tekstu i ustawienie go w odpowiednim miejscu
    void refresh();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    // posób wyrównania tekstu w poziomie
	Alignment alignment_horizontal;
    
    // label tekstu opisujacy co wyświetla to pole tekstowe
    Textfield label;

    // obiekt z tekstem do wyświetlenia
    Textfield textfield;

    // tło stanowiące ramkę wewnątrz której jest tekst
    sf::RectangleShape background;

    // wartość marginesu używanego w przypadku wyrównania tekstu do krawędzi
    const int margin = 12;
};

#endif /* UI_TEXTBOX_H_ */