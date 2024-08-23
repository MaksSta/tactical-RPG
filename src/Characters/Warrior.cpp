#include "Warrior.h"

Warrior::Warrior(sf::Vector2i start_coords)
  :
  CharacterOnBoard(start_coords,
                   "Warrior",
                   Team::player,
                   15,
                   {-2, -2},
                   2)
{
  finish_turn_button.desc = "Finish turn";
  finish_turn_button.img_file_path = "data/UI/finish_turn.png";

  Button_data button_attack1_data;
  button_attack1_data.desc = "Melee attack";
  button_attack1_data.img_file_path = "data/UI/warrior/attack_melee.png";
  button_attack1_data.id_name = "attack_melee";

  button_data_for_attack[Character::Activity::attack1] = button_attack1_data;

  attack.push_back({Character::Activity::attack1, Attack::CallType::targetable, 2, 1, 1, 3, 4});

  Button_data button_attack3_data;
  button_attack3_data.desc = "Power attack";
  button_attack3_data.img_file_path = "data/UI/warrior/attack3.png";
  button_attack3_data.id_name = "attack_melee2";

  button_data_for_attack[Character::Activity::attack3] = button_attack3_data;

  attack.push_back({Character::Activity::attack3, Attack::CallType::targetable, 5, 1, 2, 5, 10});

  Button_data button_attack2_data;
  button_attack2_data.desc = "Ground attack [aoe]";
  button_attack2_data.img_file_path = "data/UI/warrior/attack2.png";
  button_attack2_data.id_name = "attack_ground";

  button_data_for_attack[Activity::attack2] = button_attack2_data;
  attack.push_back({Activity::attack2, Attack::CallType::instant, 3, 1, 2, 2, 3, true});

  const int w = 48;
  const int h = 48;

  const int frames_idle = 5;
  const float fps_idle = frames_idle * 1.125;

  loadAnimation(Activity::idle, left,  "data/Warrior/Left/Png/WarriorLeftIdle.png",   frames_idle, fps_idle, w, h);
  loadAnimation(Activity::idle, right, "data/Warrior/Right/Png/WarriorRightIdle.png", frames_idle, fps_idle, w, h);
  loadAnimation(Activity::idle, up,    "data/Warrior/Up/Png/WarriorUpIdle.png",       frames_idle, fps_idle, w, h);
  loadAnimation(Activity::idle, down,  "data/Warrior/Down/Png/WarriorDownIdle.png",   frames_idle, fps_idle, w, h);

  const int frames_move = 8;
  const float fps_move = frames_move * 2.5;

  loadAnimation(Activity::move, left,  "data/Warrior/Left/Png/WarriorLeftWalk.png",   frames_move, fps_move, w, h);
  loadAnimation(Activity::move, right, "data/Warrior/Right/Png/WarriorRightWalk.png", frames_move, fps_move, w, h);
  loadAnimation(Activity::move, up,    "data/Warrior/Up/Png/WarriorUpWalk.png",       frames_move, fps_move, w, h);
  loadAnimation(Activity::move, down,  "data/Warrior/Down/Png/WarriorDownWalk.png",   frames_move, fps_move, w, h);

  const int frames_hurt = 4;
  const float fps_hurt = frames_hurt * 2.5;

  loadAnimation(Activity::hurt, left,  "data/Warrior/Left/Png/WarriorLeftHurt.png",     frames_hurt, fps_hurt, w, h);
  loadAnimation(Activity::hurt, right,   "data/Warrior/Right/Png/WarriorRightHurt.png", frames_hurt, fps_hurt, w, h);
  loadAnimation(Activity::hurt, up,    "data/Warrior/Up/Png/WarriorUpHurt.png",         frames_hurt, fps_hurt, w, h);
  loadAnimation(Activity::hurt, down,  "data/Warrior/Down/Png/WarriorDownHurt.png",     frames_hurt, fps_hurt, w, h);

  const int frames_death = 5;
  const float fps_death = frames_death * 2;

  loadAnimation(Activity::death, left,   "data/Warrior/Left/Png/WarriorLeftDeath.png",   frames_death, fps_death, w, h);
  loadAnimation(Activity::death, right,  "data/Warrior/Right/Png/WarriorRightDeath.png", frames_death, fps_death, w, h);
  loadAnimation(Activity::death, up,     "data/Warrior/Up/Png/WarriorUpDeath.png",       frames_death, fps_death, w, h);
  loadAnimation(Activity::death, down,   "data/Warrior/Down/Png/WarriorDownDeath.png",   frames_death, fps_death, w, h);

  const int frames_attack1 = 6;
  const float fps_attack1 = frames_attack1 * 2.5;

  loadAnimation(Activity::attack1, left,   "data/Warrior/Left/Png/WarriorLeftAttack01.png",   frames_attack1, fps_attack1, w, h);
  loadAnimation(Activity::attack1, right,  "data/Warrior/Right/Png/WarriorRightAttack01.png", frames_attack1, fps_attack1, w, h);
  loadAnimation(Activity::attack1, up,     "data/Warrior/Up/Png/WarriorUpAttack01.png",       frames_attack1, fps_attack1, w, h);
  loadAnimation(Activity::attack1, down,   "data/Warrior/Down/Png/WarriorDownAttack01.png",   frames_attack1, fps_attack1, w, h);

  const int frames_attack2 = 6;
  const float fps_attack2 = frames_attack2 * 2;

  loadAnimation(Activity::attack2, left,   "data/Warrior/Left/Png/WarriorLeftAttack02.png",   frames_attack2, fps_attack2, w, h);
  loadAnimation(Activity::attack2, right,  "data/Warrior/Right/Png/WarriorRightAttack02.png", frames_attack2, fps_attack2, w, h);
  loadAnimation(Activity::attack2, up,     "data/Warrior/Up/Png/WarriorUpAttack02.png",       frames_attack2, fps_attack2, w, h);
  loadAnimation(Activity::attack2, down,   "data/Warrior/Down/Png/WarriorDownAttack02.png",   frames_attack2, fps_attack2, w, h);

  const int frames_attack3 = 5;
  const float fps_attack3 = frames_attack3 * 2;

  loadAnimation(Activity::attack3, left,  "data/Warrior/Left/Png/WarriorLeftAttack03.png",   frames_attack3, fps_attack3, w, h);
  loadAnimation(Activity::attack3, right, "data/Warrior/Right/Png/WarriorRightAttack03.png", frames_attack3, fps_attack3, w, h);
  loadAnimation(Activity::attack3, up,    "data/Warrior/Up/Png/WarriorUpAttack03.png",       frames_attack3, fps_attack3, w, h);
  loadAnimation(Activity::attack3, down,  "data/Warrior/Down/Png/WarriorDownAttack03.png",   frames_attack3, fps_attack3, w, h);

  // zainicjowanie postaci na ekranie z domyślnymi ustawieniami
  init();
}
