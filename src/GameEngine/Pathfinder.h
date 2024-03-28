/** 
 * Celem klasy jest znalezienie najkrótszej ścieżki między dwoma polami
 * używany jest w tym celu algorytm A*
 * dodatkowo uwzględniana jest maksymalna odległóść jaką może mieć ścieżka
 * 
 * pomocniczo używany jest też algorytm sprawdzający czy jest możliwość dojścia
 * z jednego pola na drugie (informacja zwrotna areConnected(p, q))
 * 
 * Schemat działania i instrukcja użycia:
 * 1. Utworzenie instancji Pathfinder podajać w argumentach:
 *      - tablicę[8][8] pól na których działa
 *      - wektor pól, które są traktowane jako przeszkody
 * 
 * 2. Wywołanie metody astar_search podając w argumentach:
 *      - pole startowe drogi
 *      - pole końcowe drogi
 *      - maksymalną długość drogi
 *  Zwrócony zostanie obiekt klasy Road z najkrószą znalezioną ścieżką
 * 
 * 3. W przypadku zmiany pól do sprawdzania czy stanu (zmiany przeszkód)
 *    wymagane jest ponowne utworzenie klasy instacji Pathfinder
*/

#ifndef GAME_ENGINE_PATHFINDER_H_
#define GAME_ENGINE_PATHFINDER_H_

#include <cmath>
#include <map>

#include "Field.h"
#include "Road.h"

enum PathState {
    Unchecked,  // początkowy stan [dostępnego] pola
    Blocked,    // niedostępne pole
    Path,       // pole znajduje się w obecnie najszybszej ściezce
    Visited,    // pole było już sprawdzane
};

class Pathfinder {
public:
    /** 
     * konstruktor do którego należy przekazać obecny stan planszy
     * @param activeFields to tablica pól, które stanowią sprawdzany obszar
     * @param blockedFields to wektor pól traktowanych jako przeszkody 
    */
    Pathfinder(Field* activeFields[8][8], std::vector<Field*> blockedFields);

    /** 
     * wywołanie algorytmu A*
     * @param start_field to początkowe pole drogi
     * @param finish_field to końcowe pole drogi
     * @param max_distance to maksymalna długość dopuszczalnej drogi
     * @return zwraca najkrószą drogę znalezieną drogę, 
     * jeśli nie ma połączenia między pola zwraca pusty Road{} 
    */
    Road astar_search(Field* start_field, Field* finish_field, int max_distance);
private:
    // tablica wskaźników do pól, na których pracuje Pathfinder
    Field* field[8][8];

    /** 
     * znalezienie zbioru sąsiadujących pól do danego
     * @param field to pole wokół którego sprawdzane są sąsiednie
     * @return zwraca wektor wskaźników do wszystkich pól w otoczeniu
    */
    std::vector<Field*> getAdjacentFields(Field* field);

    /** 
     * próba znalezienia pola w odległości względem podanego
     * @param field_a to pole, względem którego chemy znaleźć inne
     * @param offset to odległośc od tego pola podanaw postaci {x, y}
     * @return zwraca pole w podanej odległości lub nullptr jeśli nie istnieje 
    */
    Field* tryGetFieldInDistance(Field* field_a, sf::Vector2i offset);

    /************* DOTYCZY ALGORYTMU SPRAWDZAJĄCEGO CZY POLA SĄ POŁĄCZONE **********/
    // tablica do identyfikacja grup (wysp) pól, identyczna wartość w id informuje, że pola są połączone
    std::map<Field*, Field*> id;

    // połączenie ze sobą dwóch pól poprzez podmianę wartości w tablicy id
    void connect(Field* p, Field* q);

    // informacja zwrotna o tym czy można poprowadzić ścieżkę z punktu p do q
    bool areConnected(Field* p, Field* q);

    /************** DOTYCZY ALGORYTMU ZNAJDUJĄCEGO NAKRÓTSZĄ DROGĘ (A*) ************/
    // stan wszystkich pól
    PathState states[8][8];

    // poprzednie pole, z którego zostało "otworzone" te podane w kluczu
    std::map<Field*, Field*> previous_field;

    // obecnie najlepsza droga, która zostanie zwrócona jako rezultat
    std::vector<Field*> path_vector;

    // odgległość wybranego pola od pola startowego
    float g_scores[8][8];

    // odgległość wybranego pola od pola końcowego
    float h_scores[8][8];

    // suma wyników z pól g i h dla każdego pola
    float f_scores[8][8];

    // pole z obecnie najniższym wynikiem f
    Field* min_f_field {nullptr};

    // najkrósza szacowana odległość po liniach
    float calculate_score(const sf::Vector2i& cell_a, const sf::Vector2i& cell_b);

    // alternatywna metoda szacowania odłełosci dla powyższej
    // traktująca odległości po przekątnych jako sqrt(2) więc preferuje ruch po skosie
    // w tym momencie nie jest używana
    float calculate_score_with_diagonals(const sf::Vector2i& cell_a, const sf::Vector2i& cell_b);
};

#endif /* GAME_ENGINE_PATHFINDER_H_ */