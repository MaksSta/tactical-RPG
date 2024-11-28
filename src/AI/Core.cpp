#include "Core.h"

using namespace AI;

Core::Core(CharacterOnBoard* _controlledCharacter,
           ActiveBoard& _board)
  :
  controlledCharacter{_controlledCharacter},
  board{&_board}
{}

Decision Core::calculateBestDecision()
{
  // skopiowanie liczby akcji na postaci, by liczyć na niej potencjalne akcje do wykonania
  AP = controlledCharacter->getAP();

  any_action_taken = false;

  // strategia dla postaci walczących wręcz
  if (controlledCharacter->getVocation() == Character::Vocation::fighter)
  {
    // priorytet 1: próba ataku
    try_attack();

    // funkcja przygotowująca możliwe ruchy do wykonania
    calculate_possible_moves();

    // priorytet 2: próba podjęcia akcji ruchu + ataku
    if (AP > 0 && !any_action_taken)
      try_move_and_attack();

    // priorytet 3: gonienie za najbliższą postacią
    if (AP > 0 && !any_action_taken)
      follow_closest_enemy();
  }

  // strategia dla postaci dystansowych
  else if (controlledCharacter->getVocation() == Character::Vocation::ranger)
  {
    // priorytet 1: próba ataku
    try_attack();

    // funkcja przygotowująca możliwe ruchy do wykonania
    calculate_possible_moves();

    // priorytet 2: próba podjęcia akcji ruchu + ataku
    if (AP > 0 && !any_action_taken)
      try_move_and_attack();

    // priorytet 3: postać kieruje się ku środkowi planszy
    if (AP > 0 && !any_action_taken)
      move_to_the_center();
  }


  // akcja zakończenia tury jeżeli nie ma już nic do zrobienia
  if (AP == 0 || !any_action_taken) {
    FinishTurn finishturn;
    decision.pushAction(finishturn);
  }

  return decision;
}

void Core::calculate_possible_moves()
{
  std::vector<std::vector<sf::Vector2i>> offsets_for_check;

  std::vector<sf::Vector2i> directions = { {-1, 0}, {1, 0}, {0, 1}, {0, -1} };

  // dodanie pierwszego ruchu
  for (auto & d : directions)
  {
    std::vector<sf::Vector2i> dd;
    dd.push_back(d);
    offsets_for_check.push_back(dd);
  }

  // dodanie wszystkich możliwych kombinacji ruchu po pierwszym ruchu
  for (int ap_used = 1; ap_used < AP; ap_used++)
  {
    auto copy_for_current_iteration = offsets_for_check;
    for (auto & previous_offsets : copy_for_current_iteration)
    {
      for (auto & d : directions) {
        // pominięcie przeciwnego ruchu do ostatniego (np. unikanie bezsensownego ruchu prawo zaraz po ruchu w lewo)
        // TODO - by uniknąć w całości ww. problemu należy porównać zapisane ostatnie akcje, jeżeli akcja kończyła się ruchem
        if (previous_offsets.back() == -d)
          continue;
        previous_offsets.push_back(d);
        offsets_for_check.push_back(previous_offsets);
      }
    }
  }

  // wyeliminowanie ścieżek, które nie prowadzą przez pola planszy lub są zajęte
  for (auto & checked_offset : offsets_for_check)
  {
    sf::Vector2i total_offset{0,0};
    bool invalid_field_in_road{false};
    bool any_field_blocked{false};

    for (auto & offset_step : checked_offset)
    {
      total_offset += offset_step;
      Field* field_in_road = board->getField(board->getCoordsOf(board->getFieldOccupedBy(controlledCharacter))
                                             + total_offset);

      // sprawdzenie czy wszystkie pole po drodze są istniejącymi polami na planszy
      if (!field_in_road)
      {
        invalid_field_in_road = true;
        break;
      }

      // sprawdzenie czy ruch po wykonaniu nie prowadzi do zajętego pola
      for (auto & occupied_field : board->getBlockedFields())
      {
        if (occupied_field == field_in_road)
        {
          any_field_blocked = true;
        }
      }
      if (any_field_blocked)
        break;
    }

    if (!invalid_field_in_road && !any_field_blocked)
      legal_offsets.push_back(checked_offset);
  }
}

void Core::try_attack()
{
    // próba podjęcia akcji ataku
  AttackInfo attackInfo = calculateBestAttack(AP);
  if (attackInfo.target != nullptr)
  {
    decision.pushAction(AI::Attack(attackInfo));
    AP -= attackInfo.attack->getAP();
    any_action_taken = true;
  }
}

void Core::try_move_and_attack()
{

  for (auto & checked_offset : legal_offsets)
  {
    sf::Vector2i total_offset{0,0};
    int steps = 0;
    for (auto & offset_step : checked_offset)
    {
      total_offset += offset_step;
      steps++;
    }

    auto possible_atack_after_move = calculateBestAttack(AP - steps, total_offset);

    if (possible_atack_after_move.target)
    {
      std::vector<Field*> fields_of_road;
      sf::Vector2i current_offset;

      for (auto &offset_steps : checked_offset) {
        current_offset += offset_steps;
        auto field_after_step = board->getField(
            board->getCoordsOf(board->getFieldOccupedBy(controlledCharacter)) +
            current_offset);
        fields_of_road.push_back(field_after_step);
      }

      Road road(fields_of_road, board->getFieldOccupedBy(controlledCharacter));

      Move test_move(road);
      decision.pushAction(test_move);
      AP -= fields_of_road.size();
      any_action_taken = true;

      // pominięcie sprawdzania pozostałych dróg - pierwsza która prowadziła do
      // celu jest zatwierdzona
      // TODO spośród wszystkich celów dostępnych w zasięgu, wybrać taki który
      // ma najmniej hp, ew. przekalkulować jeszcze ile wyjdzie na to AP
      // (obecnie sprawdza z pierszego zasięgu z listy)
      break;
    }
  }
}

void Core::follow_closest_enemy()
{
  int best_dst = 63;
  std::vector<Field*> best_road_to_enemy;
  std::vector<sf::Vector2i> enemy_positions;

  std::vector<CharacterOnBoard*> targets; // narazie niepotrzebne, bo liczy się sama odglełość, potem będzie używane przy innych czynnikach

  for (auto &c : board->getAliveCharacters())
  {
    if (c->getTeam() != controlledCharacter->getTeam())
    {
      targets.push_back(c);
      enemy_positions.push_back(c->getLocalCoords());
    }
  }

  // NOTE od tego miejsca kod powielony z move_to_the_center tylko podmienione nazwy 2 powyższych zmiennych
  // sprawdzanie wszytkich kombinacji ruchu od najkrószej - szukanie drogi najbliżej centrum
  for (auto & checked_offset : legal_offsets)
  {
    // kolejne mijane pola w drodze
    std::vector<Field*> fields;

    sf::Vector2i current_offset;
    for (auto & offset_steps : checked_offset)
    {
      current_offset += offset_steps;
      auto field_after_step = board->getField(board->getCoordsOf(board->getFieldOccupedBy(controlledCharacter))
                                              + current_offset);
      fields.push_back(field_after_step);
    }

    sf::Vector2i total_offset = current_offset;

    auto pos = board->getCoordsOf(board->getFieldOccupedBy(controlledCharacter));

    int current_best_dist = 63;
    for (auto & cf : enemy_positions)
    {
      sf::Vector2i vec_to_center = (pos + total_offset) - cf;
      int dist_to_center = abs(vec_to_center.x) + abs(vec_to_center.y);
      if (dist_to_center < current_best_dist)
        current_best_dist = dist_to_center;
    }

    if (current_best_dist < best_dst)
    {
      best_dst = current_best_dist;
      best_road_to_enemy = fields;
    }
  }
  // wybranie pierwszego spośród ruchów, który prowadził najbliżej centrum
  Road road(best_road_to_enemy, board->getFieldOccupedBy(controlledCharacter));

  Move test_move(road);
  decision.pushAction(test_move);
  AP -= best_road_to_enemy.size();
  any_action_taken = true;
}

void Core::move_to_the_center()
{
  int best_dst = 63;
  std::vector<Field*> best_road_to_center;

  std::vector<sf::Vector2i> centerfields = { {3, 3}, {3, 4}, {4, 3}, {4, 4} };

  // sprawdzanie wszytkich kombinacji ruchu od najkrószej - szukanie drogi najbliżej centrum
  for (auto & checked_offset : legal_offsets)
  {
    // kolejne mijane pola w drodze
    std::vector<Field*> fields;

    sf::Vector2i current_offset;
    for (auto & offset_steps : checked_offset)
    {
      current_offset += offset_steps;
      auto field_after_step = board->getField(board->getCoordsOf(board->getFieldOccupedBy(controlledCharacter))
                                              + current_offset);
      fields.push_back(field_after_step);
    }

    sf::Vector2i total_offset = current_offset;

    auto pos = board->getCoordsOf(board->getFieldOccupedBy(controlledCharacter));

    int current_best_dist = 63;
    for (auto & cf : centerfields)
    {
      sf::Vector2i vec_to_center = (pos + total_offset) - cf;
      int dist_to_center = abs(vec_to_center.x) + abs(vec_to_center.y);
      if (dist_to_center < current_best_dist)
        current_best_dist = dist_to_center;
    }

    if (current_best_dist < best_dst)
    {
      best_dst = current_best_dist;
      best_road_to_center = fields;
    }
  }
  // wybranie pierwszego spośród ruchów, który prowadził najbliżej centrum
  Road road(best_road_to_center, board->getFieldOccupedBy(controlledCharacter));

  Move test_move(road);
  decision.pushAction(test_move);
  AP -= best_road_to_center.size();
  any_action_taken = true;
}

AttackInfo Core::calculateBestAttack(int AP,
                                     sf::Vector2i offset_from_character_pos)
{
  CharacterOnBoard* target_with_lowest_hp{nullptr};
  Abilities::Attack* chosen_attack{nullptr};
  Range range_of_attack;

  // wybranie pierwszego dostępnego ataku, na które starcza punktów akcji
  for (auto &attack : controlledCharacter->getAttacks())
  {
    if (AP < attack.getAP())
      continue;

    auto modified_in_range = attack.get_in_range();

    if (offset_from_character_pos != sf::Vector2i{0, 0})
    {
      for (auto & rd : modified_in_range)
      {
        rd += offset_from_character_pos;
      }
    }

    range_of_attack = board->createRange(controlledCharacter, modified_in_range);
    target_with_lowest_hp = tryFindTargetWithLowestHP(range_of_attack);
    chosen_attack = &attack;

    // pominięcie sprawdzania dalszych rodzajów ataków (priorytetując w ten sposób wykonanie ataku nr 1, potem 2 itd.)
    if (target_with_lowest_hp)
      break;
  }
  
  return {chosen_attack, target_with_lowest_hp, range_of_attack};
}

CharacterOnBoard* Core::tryFindTargetWithLowestHP(Range& range)
{
  std::vector<CharacterOnBoard*> targets;

  // dodanie do listy celów wszystkich postaci z przeciwnej drużyny znajdujących się w zasięgu wywołania ataku
  for (auto &c : board->getAliveCharacters())
  {
    if ((board->isFieldInRange(board->getFieldOccupedBy(c), range)) && (c->getTeam() != controlledCharacter->getTeam()))
    {
      targets.push_back(c);
    }
  }

  // znalezienie spośród celów tego o najniższej ilości hp
  CharacterOnBoard* target_with_lowest_hp{nullptr};
  int lowest_hp = 9999;
  for (auto &t : targets)
  {
    if (t->getHP() < lowest_hp)
    {
      target_with_lowest_hp = t;
      lowest_hp = t->getHP();
    }
  }

  return target_with_lowest_hp;
}
