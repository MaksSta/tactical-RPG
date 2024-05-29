#include "GoblinSlinger.h"

GoblinSlinger::GoblinSlinger(sf::Vector2i start_coords) 
:
	CharacterOnBoard(	"Goblin slinger",
                        Team::enemy,
						10,
						start_coords,
						{-2, -2},
						2
					)
{
	attack.push_back({
		Attack(Character::Activity::attack1, Attack::CallType::targetable, 3, 2, 4, 2, 4),
	});

	const int w = 48;
	const int h = 48;

	const int frames_idle = 6;
	const float fps_idle = frames_idle * 1.125;

	loadAnimation(Character::idle, left,	"data/GoblinSlinger/Left/Png/GoblinSlingerLeftIdle.png",		frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, right, 	"data/GoblinSlinger/Right/Png/GoblinSlingerRightIdle.png",		frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, up,		"data/GoblinSlinger/Up/Png/GoblinSlingerUpIdle.png",			frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, down, 	"data/GoblinSlinger/Down/Png/GoblinSlingerDownIdle.png",		frames_idle, fps_idle,	w,	h);

	const int frames_move = 6;
	const float fps_move = frames_move * 2.5;

	loadAnimation(Character::move, left, 	"data/GoblinSlinger/Left/Png/GoblinSlingerLeftRun.png",		frames_move, fps_move,	w,	h);
	loadAnimation(Character::move, right, 	"data/GoblinSlinger/Right/Png/GoblinSlingerRightRun.png",	frames_move, fps_move,	w,	h);
	loadAnimation(Character::move, up, 		"data/GoblinSlinger/Up/Png/GoblinSlingerUpRun.png",			frames_move, fps_move,	w,	h);
	loadAnimation(Character::move, down, 	"data/GoblinSlinger/Down/Png/GoblinSlingerDownRun.png",		frames_move, fps_move,	w,	h);
	
	const int frames_attack1 = 21;
	const float fps_attack1 = frames_attack1 * 1;

	loadAnimation(Character::attack1, left, 	"data/GoblinSlinger/Left/Png/GoblinSlingerLeftAttack01.png",	frames_attack1, fps_attack1,	w,	h);
	loadAnimation(Character::attack1, right, 	"data/GoblinSlinger/Right/Png/GoblinSlingerRightAttack01.png",	frames_attack1, fps_attack1,	w,	h);
	loadAnimation(Character::attack1, up, 		"data/GoblinSlinger/Up/Png/GoblinSlingerUpAttack01.png",		frames_attack1, fps_attack1,	w,	h);
	loadAnimation(Character::attack1, down, 	"data/GoblinSlinger/Down/Png/GoblinSlingerDownAttack01.png",	frames_attack1, fps_attack1,	w,	h);

	const int frames_hurt = 4;
	const float fps_hurt = frames_hurt * 2.5;

	loadAnimation(Character::hurt, left, 	"data/GoblinSlinger/Left/Png/GoblinSlingerLeftHurt.png",		frames_hurt, fps_hurt,	w,	h);
	loadAnimation(Character::hurt, right, 	"data/GoblinSlinger/Right/Png/GoblinSlingerRightHurt.png",		frames_hurt, fps_hurt,	w,	h);
	loadAnimation(Character::hurt, up, 		"data/GoblinSlinger/Up/Png/GoblinSlingerUpHurt.png",			frames_hurt, fps_hurt,	w,	h);
	loadAnimation(Character::hurt, down, 	"data/GoblinSlinger/Down/Png/GoblinSlingerDownHurt.png",		frames_hurt, fps_hurt,	w,	h);

	// zainicjowanie postaci na ekranie z domyślnymi ustawieniami
	init();
}