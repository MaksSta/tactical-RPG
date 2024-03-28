/**
 * Akcja przesunięcia obiektu w czasie
*/

#ifndef ANIMATIONS_ACTIONS_MOVE_H_
#define ANIMATIONS_ACTIONS_MOVE_H_

#include <SFML/Graphics.hpp>

#include "../../Global.h"

namespace Animations
{
    namespace Actions 
    {
        class Move {
        public:
            /** 
             * akcja przesunięcia o podany wektor w czasie
             * @param _offset przesunięcie jakie jest do wykonania
             * @param _speed prędkość przesunięcia w pikselach/sekundę
             */ 
            Move(sf::Vector2f _offset, float _speed);
            
            // przesunięcie jakie ma zostać wykonane
            sf::Vector2f offset;

            // prędkość przesunięcia w pikselach/sekundę
            float speed;

            // kierunek wykonywania ruchu
            Direction direction;
        };

        // wyjątki do ustalenia niezmienników
        namespace errors
        {
            struct zero_move {};
            struct zero_speed {};
            struct diagonal_move {};
        }
        
    }

}

#endif /* ANIMATIONS_ACTIONS_MOVE_H_ */