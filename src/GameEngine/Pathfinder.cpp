#include "Pathfinder.h"

Pathfinder::Pathfinder(Field* activeFields[8][8], std::vector<Field*> blockedFields)
{
    for(int x = 0; x < 8; x++)
        for(int y = 0; y < 8; y++)
        {
            // przypisanie przekazanej tablicy pól wskaźników
            field[x][y] = activeFields[x][y];

            // nadanie wszystkim polom osobnych id (z adresami ich samych)
            id[field[x][y]] = field[x][y];

            if (std::find(blockedFields.begin(), blockedFields.end(), field[x][y] ) != blockedFields.end())
                // ustawienie stanu pól zablokowanych pól podanych
                states[x][y] = PathState::Blocked;
            else
            {
                // ustawianie stanu pól na domyślną wartość (niesprawdzane)
                states[x][y] = PathState::Unchecked;
            }
        }

    // połączenie ze sobą wszystkich sąsiadujących pól
    for(int x = 0; x < 8; x++)
        for(int y = 0; y < 8; y++)
        {
            if (states[x][y] == PathState::Blocked)
                continue;

            auto adjacentFields = getAdjacentFields(field[x][y]);
            for (auto adj : adjacentFields)
                connect(field[x][y], adj);
        }
}

Road Pathfinder::astar_search(Field* start_field, Field* finish_field, int max_distance)
{
    /******************************************************************************
    **** Wstępne upewnienie się czy pola początkowe i końcowe można  połączyć  ****
    ******************************************************************************/
    if (!areConnected(start_field, finish_field) )
        return Road{};

    /******************************************************************************
    ********** Znalezienie najkrótszej drogi korzystając z alogrytmu A*  **********
    ******************************************************************************/

    // zainicijowanie wszystkich danych
	previous_field.clear();

    path_vector.clear();
	path_vector.push_back(start_field);

    // przypisanie początkowych wartości dla wszystkich pól
	for (unsigned short x = 0; x < 8; x++) {
		for (unsigned short y = 0; y < 8; y++)
		{
            // ustawienie odległości od pola końcowego dla wszystkich pól
			h_scores[x][y] = calculate_score(sf::Vector2i(x, y), finish_field->getCoords());

            // dla pola startowego:
			if (x == start_field->getCoords().x && y == start_field->getCoords().y)
			{
                // f (łączna odlegość) ma wartość odległości od pola końcowego
				f_scores[x][y] = h_scores[x][y];

                // g (odl. od startowego pola) ma wartość 0
				g_scores[x][y] = 0;
			}
            // dla wszystkich innych pól:
			else
			{
                // g (odl. od startowego pola) i f (łączny dystans) incijowane są maksymalną możliwą wartością
                // ich wartość będzie przeliczona dopiero w momencie ich "otwarcia"
				f_scores[x][y] = 8 * 8;
				g_scores[x][y] = 8 * 8;
			}
		}
	}

    bool astar_finished = false;

    // kontynuowanie wykonywania algorytmu A* dopóki droga nie zostanie odnaleziona
    while (!astar_finished) 
    {
        // ustawienie iteratora wskazującego ostatnie pole najkrószej ściezki na pole startowe
        std::vector<Field*>::iterator min_f_field_iterator = path_vector.begin();

        for (std::vector<Field*>::iterator p = 1 + path_vector.begin(); p != path_vector.end(); p++)
        {
            sf::Vector2i a = (*p)->getCoords();
            // porównywane są kolejne wyniki f, aż do napotkania elementu z minimum
            if (f_scores[a.x][a.y] < 
                f_scores[(*min_f_field_iterator)->getCoords().x][(*min_f_field_iterator)->getCoords().y])
            {
                min_f_field_iterator = p;
            }
        }

        min_f_field = *min_f_field_iterator; 

        // wyjęcie pola z obecnie najniższym wynikiem f z poszukiwanej drogi...
        path_vector.erase(min_f_field_iterator);
        // ...i oznaczenie go jako przebyty
        states[min_f_field->getCoords().x][min_f_field->getCoords().y] = PathState::Visited;

        // najkrósza droga pokazuje już na końcowe pole == zakończono
        if (min_f_field->getCoords() == finish_field->getCoords())
            astar_finished = true;

        // "otworzenie" sąsiadujących pól dla tego z obecnie najlepszym wynikiem
        for (Field* adjacent_field : getAdjacentFields(min_f_field))
        {
            sf::Vector2i adjacent_cell = adjacent_field->getCoords();

            // rozpatrywane są tylko pola, które nie zostały jeszcze oznaczonego jako visited przy sprawdzaniu
            if (states[adjacent_cell.x][adjacent_cell.y] != PathState::Visited) 
            {
                // pobranie obecnej wartość odległości od pola startowego
                float temp_g_score = g_scores[min_f_field->getCoords().x][min_f_field->getCoords().y];
                
               // powiększenie tej odlełości o 1
               temp_g_score++;

                // znalezienie pola z najlepszym wynikiem g (odgległość od pola startowego)
                if (temp_g_score < g_scores[adjacent_cell.x][adjacent_cell.y])
                {
                    // zmapowanie pole z którego zostało ono otworzone
                    previous_field[adjacent_field] = min_f_field;

                    // nadpisanie wartości całkowitego dystansu
                    f_scores[adjacent_cell.x][adjacent_cell.y] = temp_g_score + h_scores[adjacent_cell.x][adjacent_cell.y];

                    // nadpisanie odległości od pola startowego
                    g_scores[adjacent_cell.x][adjacent_cell.y] = temp_g_score;

                    // dopisanie pola do ścieżki
                    path_vector.push_back(adjacent_field);
                }
            }
        }
    }
    /***********************************************************************************
    ** Odczytanie najlepszej znalezionej drogi i zapisanie jej w odwrotnej kolejności **
    ***********************************************************************************/
    std::vector<Direction> road_directions;
    std::vector<Field*> road_fields;

    // wektor pól tworzących najkrótszą ścieżkę - reeversed, ponieważ idzie od punktu docelowego aż do postaci
    // dodatkowo zapisywane są kierunki w jakich się poruszał
    std::vector<Direction> reversed_directions;
    std::vector<Field*> reversed_road;

    // ustawienie wskaźnika pola na końcowy element, by iterować przez previous_field jak po liście
    Field* path_field = min_f_field;

	unsigned short path_length = 0;
    do {
        path_length++;
        // odrzucenie wyniku w którym przekroczono maksymalną ilość pól do przesunięcia
        if(path_length > max_distance)
            return {};
            
        // rozponanie kierunku ścieżki - przeciwny kierunek do poprzedniego pola
        Direction direction;
        if (previous_field[path_field] == tryGetFieldInDistance(path_field, sf::Vector2i{-1, 0}))
            direction = right;
        else  if (previous_field[path_field] == tryGetFieldInDistance(path_field, sf::Vector2i{1, 0}))
            direction = left;
        else  if (previous_field[path_field] == tryGetFieldInDistance(path_field, sf::Vector2i{0, -1}))
            direction = down;
        else  if (previous_field[path_field] == tryGetFieldInDistance(path_field, sf::Vector2i{0, 1}))
            direction = up;

        reversed_road.push_back({path_field});
        reversed_directions.push_back({direction});

        states[path_field->getCoords().x][path_field->getCoords().y] = PathState::Path;

        // iterowanie po liście pól wstecz aż do pola startowego 
        path_field = previous_field[path_field];
    } while (path_field != start_field);

    // odwrócenie drogi - od pierwszego pola do punktu docelowego z wyłączeniem pnuktu startowego (bo nie stanowi on drogi)
    for( auto r = reversed_road.end() - 1 ; r >= reversed_road.begin(); r-- )
        road_fields.push_back(*r);

    for( auto r = reversed_directions.end() - 1 ; r >= reversed_directions.begin(); r-- )
        road_directions.push_back(*r);

    return {road_fields, start_field};
}

std::vector<Field*> Pathfinder::getAdjacentFields(Field* field)
{
    // zwracana lista - pola w sąsiedztwie
    std::vector<Field*> adjacentFields;

    // lista wskaźników pól w sąsiedztwie, jeśli nie istenieje takie pole, również dopisane jest nullptr
    std::vector<Field*> possible_neightbours;
    
    possible_neightbours.push_back(tryGetFieldInDistance(field, sf::Vector2i{-1, 0}));
    possible_neightbours.push_back(tryGetFieldInDistance(field, sf::Vector2i{1, 0}));
    possible_neightbours.push_back(tryGetFieldInDistance(field, sf::Vector2i{0, 1}));
    possible_neightbours.push_back(tryGetFieldInDistance(field, sf::Vector2i{0, -1}));

    // przeszukanie listy możliwych sąsiadów
    for( auto next_to : possible_neightbours) {
        if (next_to == nullptr)
            continue;

        PathState state_of_neightbour = states[next_to->getCoords().x][next_to->getCoords().y];

        if (state_of_neightbour != PathState::Blocked)
            adjacentFields.push_back(next_to);
    }
        
	return adjacentFields;
}


Field* Pathfinder::tryGetFieldInDistance(Field* field_a, sf::Vector2i offset)
{
    auto new_pos = field_a->getCoords() + offset;

    if (new_pos.x < 0 || new_pos.x >= 8 || new_pos.y < 0 || new_pos.y >= 8)
        return nullptr;

    return field[new_pos.x][new_pos.y];
}

void Pathfinder::connect(Field* p, Field* q)
{
    auto pid = id[p];
    auto qid = id[q];

    // odnalezieniego każdego elementu z wartością p i zamiana na wartość q
    for (auto & d : id)
        if(d.second == pid)
            d.second = qid;
}

bool Pathfinder::areConnected(Field* p, Field* q)
{
    if (id[p] == id[q])
        return true;

    return false;
}

float Pathfinder::calculate_score(const sf::Vector2i& cell_a, const sf::Vector2i& cell_b)
{
    return abs(cell_b.x - cell_a.x) + abs(cell_b.y - cell_a.y);
}

float Pathfinder::calculate_score_with_diagonals(const sf::Vector2i& cell_a, const sf::Vector2i& cell_b)
{
    float distance_x = abs(cell_b.x - cell_a.x);
	float distance_y = abs(cell_b.y - cell_a.y);

    // wyliczenie odległości od celu - po skosie od którszej współrzędnej + reszta dystansu po linii
	return      std::min(distance_x, distance_y) * sqrt(2)
            +   std::max(distance_x, distance_y) - std::min(distance_x, distance_y);
}