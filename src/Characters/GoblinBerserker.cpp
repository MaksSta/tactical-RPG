#include "GoblinBerserker.h"

GoblinBerserker::GoblinBerserker(sf::Vector2i start_coords)
  :
  CharacterOnBoard(start_coords,
                   "Goblin Berserker",
                   Vocation::fighter,
                   Team::enemy,
                   7,
                   {-2, -2},
                   2)
{
  // Activity::attack1, rodzaj ataku, koszt akcji, min range, max range, min dmg, max dmg
  attack.push_back({Activity::attack1, Abilities::Attack::CallType::targetable, 3, 1, 1, 3, 7});

  const int w = 48;
  const int h = 48;

  const int frames_idle = 6;
  const float fps_idle = frames_idle * 1.5;

  loadAnimation(Activity::idle, left, "data/GoblinBerserker/Left/Png/GoblinLeftIdle.png", frames_idle, fps_idle, w, h);
  loadAnimation(Activity::idle, right, "data/GoblinBerserker/Right/Png/GoblinRightIdle.png", frames_idle, fps_idle, w, h);
  loadAnimation(Activity::idle, up, "data/GoblinBerserker/Up/Png/GoblinUpIdle.png", frames_idle, fps_idle, w, h);
  loadAnimation(Activity::idle, down, "data/GoblinBerserker/Down/Png/GoblinDownIdle.png", frames_idle, fps_idle, w, h);

  const int frames_move = 6;
  const float fps_move = frames_move * 1.5;

  loadAnimation(Activity::move, left, "data/GoblinBerserker/Left/Png/GoblinLeftRun.png", frames_move, fps_move, w, h);
  loadAnimation(Activity::move, right, "data/GoblinBerserker/Right/Png/GoblinRightRun.png", frames_move, fps_move, w, h);
  loadAnimation(Activity::move, up, "data/GoblinBerserker/Up/Png/GoblinUpRun.png", frames_move, fps_move, w, h);
  loadAnimation(Activity::move, down, "data/GoblinBerserker/Down/Png/GoblinDownRun.png", frames_move, fps_move, w, h);

  const int frames_hurt = 3;
  const float fps_hurt = frames_hurt * 2.75;

  loadAnimation(Activity::hurt, left, "data/GoblinBerserker/Left/Png/GoblinLeftHurt.png", frames_hurt, frames_hurt, w, h);
  loadAnimation(Activity::hurt, right, "data/GoblinBerserker/Right/Png/GoblinRightHurt.png", frames_hurt, fps_hurt, w, h);
  loadAnimation(Activity::hurt, up, "data/GoblinBerserker/Up/Png/GoblinUpHurt.png", frames_hurt, fps_hurt, w, h);
  loadAnimation(Activity::hurt, down, "data/GoblinBerserker/Down/Png/GoblinDownHurt.png", frames_hurt, fps_hurt, w, h);

  const int frames_death = 9;
  const float fps_death = frames_death * 1.5;

  loadAnimation(Activity::death, left, "data/GoblinBerserker/Left/Png/GoblinLeftDeath.png", frames_death, fps_death, w, h);
  loadAnimation(Activity::death, right, "data/GoblinBerserker/Right/Png/GoblinRightDeath.png", frames_death, fps_death, w, h);
  loadAnimation(Activity::death, up, "data/GoblinBerserker/Up/Png/GoblinUpDeath.png", frames_death, fps_death, w, h);
  loadAnimation(Activity::death, down, "data/GoblinBerserker/Down/Png/GoblinDownDeath.png", frames_death, fps_death, w, h);

  const int frames_attack1 = 10;
  const float fps_attack1 = frames_attack1 * 1;

  loadAnimation(Activity::attack1, left, "data/GoblinBerserker/Left/Png/GoblinLeftAttack01.png", frames_attack1, fps_attack1, w, h);
  loadAnimation(Activity::attack1, right, "data/GoblinBerserker/Right/Png/GoblinRightAttack01.png", frames_attack1, fps_attack1, w, h);
  loadAnimation(Activity::attack1, up, "data/GoblinBerserker/Up/Png/GoblinUpAttack01.png", frames_attack1, fps_attack1, w, h);
  loadAnimation(Activity::attack1, down, "data/GoblinBerserker/Down/Png/GoblinDownAttack01.png", frames_attack1, fps_attack1, w, h);


  //zainicjowanie postaci na ekranie z domyślnymi ustawieniami
  init();
}
