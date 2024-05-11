/** 
 * przycisk o załadowanej grafice obsługiwany przez UI
 * zawiera informację o rodzaju wywoływanej akcji
 * jeżeli jest to atak, zawiera wskaźnik do uruchamianej umiejętności
*/

#ifndef UI_BUTTON_H_
#define UI_BUTTON_H_

#include <SFML/Graphics.hpp>

#include "../Abilities/Attack.h"


class Button : public sf::Sprite {
public:
    // rodzaj akcji wywoływanej przyciskiem
    enum Action {
        attack,
        endturn,
    };

    // rodzaj przycisku pod wzgłedem sposobu wywołania, a konkretniej akcji po kliknięciu
    enum ActivationType {
        selectable,
        clickable,
    };

    // stan jaki może przybierać przycisk, decyduje o jego wyglądzie
    enum btnState {
        inactive,
        hovered,
        simulated_hovered,
        selected
    };

    /**
     * Utworzenie przycisku na ekranie
     * \param _ability wskaźnik do umietności, którą wywołuje przycisk
     * \param pos pozycja na ekranie
     * \param img_file_path ścieżka do grafiki dla tego przycisku
     * \param _desc opis przycisku, który wyświetla się po najechaniu myszką
    */
    Button( ActivationType activationType,
            Attack* _ability,
            sf::Vector2f pos,
            std::string img_file_path,
            std::wstring _desc);

    Button( ActivationType activationType,
            Action _Action,
            sf::Vector2f pos,
            std::string img_file_path,
            std::wstring _desc);

    void Update(float deltaTime);

    // zwraca wskaźnik do wywoływanej akcji
    // TODO powinno zamiast Attack* zwracać typ podstawowy (Ability*)
    Attack* getAbility();

    // zwraca rodzaj akcji jaką powoduje wciśnięcie przycisku
    Action getAction();

    // zwraca rodzaj akcji jaką powoduje wciśnięcie przycisku
    ActivationType getActivationType();

    // zwraca opis przypisany do przycisku (opis wywoływanej akcji)
    std::wstring getDesc();

    friend class UI;
private:
    // rodzaj akcji jaką powoduje wciśnięcie przycisku
    Action action;

    // rodzaj interakcji jaką powoduje wciśnięcie przycisku
    ActivationType activationType;

    // stan w jakim znajduje się przycisk
    btnState state {inactive};
    
    // wskaźnik do umiejętności jaką wywoła wciśniecie przycisku
    // TODO powinien to być typ podstawowy (Ability*)
    Attack* ability;

    // opis umiejętności wykonywanej przyciskiem
    std::wstring desc;

    // tekstura służaca jako grafika pod przycisk do załadowania z pliku
    sf::Texture texture;
};

#endif  /* UI_BUTTON_H_ */