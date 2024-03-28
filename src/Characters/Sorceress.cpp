#include "Sorceress.h"

Sorceress::Sorceress(sf::Vector2i start_coords)
:
	CharacterOnBoard(	"Sorceress",
						12,
						start_coords,
						{-2, -2},
						2 
					)
{
	Button_data button_attack1_data;
	button_attack1_data.desc = L"Fireball";
	button_attack1_data.img_file_path = "data/UI/sorceress/fireball.png";
	button_attack1_data.id_name = "fireball";

	attack_full_data.push_back({
		Attack(3, 2, 3),
		Character::Activity::attack1,
		button_attack1_data
	});

	const int w = 48;
	const int h = 48;

	const int frames_idle = 6;
	const float fps_idle = frames_idle * 1.125;

	loadAnimation(Character::idle, left, 	"data/Sorceress/Left/Png/SorceressLeftIdle.png",	frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, right, 	"data/Sorceress/Right/Png/SorceressRightIdle.png",	frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, up, 		"data/Sorceress/Up/Png/SorceressUpIdle.png",		frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, down, 	"data/Sorceress/Down/Png/SorceressDownIdle.png",	frames_idle, fps_idle,	w,	h);

	const int frames_move = 6;
	const float fps_move = frames_move * 2.5;

	loadAnimation(Activity::move, left,	 	"data/Sorceress/Left/Png/SorceressLeftRun.png",		frames_move, fps_move, w, h);
	loadAnimation(Activity::move, right, 	"data/Sorceress/Right/Png/SorceressRightRun.png",	frames_move, fps_move, w, h);
	loadAnimation(Activity::move, up,		"data/Sorceress/Up/Png/SorceressUpRun.png",			frames_move, fps_move, w, h);
	loadAnimation(Activity::move, down,		"data/Sorceress/Down/Png/SorceressDownRun.png",		frames_move, fps_move, w, h);

	const int frames_attack1 = 5;
	const float fps_attack1 = frames_attack1 * 2.5;

	loadAnimation(Activity::attack1, left,	"data/Sorceress/Left/Png/SorceressLeftAttack01.png",	frames_attack1, fps_attack1, w, h);
	loadAnimation(Activity::attack1, right,	"data/Sorceress/Right/Png/SorceressRightAttack01.png",	frames_attack1, fps_attack1, w, h);
	loadAnimation(Activity::attack1, up,		"data/Sorceress/Up/Png/SorceressUpAttack01.png",	frames_attack1, fps_attack1, w, h);
	loadAnimation(Activity::attack1, down,	"data/Sorceress/Down/Png/SorceressDownAttack01.png",	frames_attack1, fps_attack1, w, h);

	const int frames_hurt = 4;
	const float fps_hurt = frames_hurt * 2.5;

	loadAnimation(Activity::hurt, left,		"data/Sorceress/Left/Png/SorceressLeftHurt.png",		frames_hurt, fps_hurt, w, h);
	loadAnimation(Activity::hurt, right,	"data/Sorceress/Right/Png/SorceressRightHurt.png",		frames_hurt, fps_hurt, w, h);
	loadAnimation(Activity::hurt, up,		"data/Sorceress/Up/Png/SorceressUpHurt.png",			frames_hurt, fps_hurt, w, h);
	loadAnimation(Activity::hurt, down,		"data/Sorceress/Down/Png/SorceressDownHurt.png",		frames_hurt, fps_hurt, w, h);

	// zainicjowanie postaci na ekranie z domyślnymi ustawieniami
	init();
}