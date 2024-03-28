/**
 * Kamera pokazująca planszę do gry,
 * rozszerzenie klasy sfml View, wzbogacone o:
 *  - efekt przesunięcia w czasie
 *  - możliwość pracy na współrzednych kafelków pól na planszy
*/

#ifndef GAME_ENGINE_CAMERA_H_
#define GAME_ENGINE_CAMERA_H_

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../Global.h"

class Camera : public sf::View {
public:
    /**
     * Utworzenie widoku kamery
     * \param size wielkość obszaru, jaki makera ma pokazywać
     * \param viewport umiejsowienie kamery na wybranym prostokącie względem wyświetlania całego ekranu jako sf::Rect
     * (cała szerokość/wysokość ekranu == 1.0f)
    */
    Camera(sf::Vector2f size, sf::FloatRect viewport);

    // ustawienie wielkości całego obszaru planszu
    void set_map_size(sf::Vector2f size);

    // ustawienie pozycji docelowej środka kamery
    void setDstPos(sf::Vector2f pos);

    // docelowe przesunięcie kamery o {x, y} pikseli
    void moveByPixels(int x, int y);

    // docelowe przesunięcie kamery o {x, y} pól na planszy
	void moveCameraByFields(int x, int y);

    void update(float delta);
private:
    // calkowity wymiar planszy [której fragment pokazuje kamera] w pikselach
    sf::Vector2f map_size;

    // docelowa pozycja na której ma się znaleźć środek kamery
	sf::Vector2f dstPos;
    
    // prędkość przemieszczania ekranu w pikselach/sek
    sf::Vector2f vel;
};

#endif /* GAME_ENGINE_CAMERA_H_ */