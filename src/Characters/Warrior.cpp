#include "Warrior.h"

Warrior::Warrior(sf::Vector2i start_coords) 
:
	CharacterOnBoard(	"Warrior",
						15,
						start_coords,
						{-2, -2},
						2
					)
{
	Button_data button_attack1_data;
	button_attack1_data.desc = L"Melee attack";
	button_attack1_data.img_file_path = "data/UI/warrior/attack_melee.png";
	button_attack1_data.id_name = "attack_melee";

	attack_full_data.push_back({
		Attack(1, 3, 4),
		Character::Activity::attack1,
		button_attack1_data
	});

	/** 
	 * TODO dodanie drugiego ataku u Warriora, wymaga nowych właściwości:
	 * - zasięg obrażeń będzie obszarowy
	 * - wywołanie będzie odbywać się na siebie
	 * - nie będzie mógł zostać wybierany jako autoatak
	*/
	Button_data button_attack2_data;
	button_attack2_data.desc = L"Atak gruntowy";
	button_attack2_data.img_file_path = "data/UI/warrior/attack2.png";
	button_attack2_data.id_name = "attack_ground";

	const int w = 48;
	const int h = 48;

	const int frames_idle = 5;
	const float fps_idle = frames_idle * 1.125;

	loadAnimation(Character::idle, left,	"data/Warrior/Left/Png/WarriorLeftIdle.png",		frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, right, 	"data/Warrior/Right/Png/WarriorRightIdle.png",		frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, up,		"data/Warrior/Up/Png/WarriorUpIdle.png",			frames_idle, fps_idle,	w,	h);
	loadAnimation(Character::idle, down, 	"data/Warrior/Down/Png/WarriorDownIdle.png",		frames_idle, fps_idle,	w,	h);

	const int frames_move = 8;
	const float fps_move = frames_move * 2.5;

	loadAnimation(Character::move, left, 	"data/Warrior/Left/Png/WarriorLeftWalk.png",		frames_move, fps_move,	w,	h);
	loadAnimation(Character::move, right, 	"data/Warrior/Right/Png/WarriorRightWalk.png",		frames_move, fps_move,	w,	h);
	loadAnimation(Character::move, up, 		"data/Warrior/Up/Png/WarriorUpWalk.png",			frames_move, fps_move,	w,	h);
	loadAnimation(Character::move, down, 	"data/Warrior/Down/Png/WarriorDownWalk.png",		frames_move, fps_move,	w,	h);
	
	const int frames_attack1 = 6;
	const float fps_attack1 = frames_attack1 * 2.5;

	loadAnimation(Character::attack1, left, 	"data/Warrior/Left/Png/WarriorLeftAttack01.png",	frames_attack1, fps_attack1,	w,	h);
	loadAnimation(Character::attack1, right, 	"data/Warrior/Right/Png/WarriorRightAttack01.png",	frames_attack1, fps_attack1,	w,	h);
	loadAnimation(Character::attack1, up, 		"data/Warrior/Up/Png/WarriorUpAttack01.png",		frames_attack1, fps_attack1,	w,	h);
	loadAnimation(Character::attack1, down, 	"data/Warrior/Down/Png/WarriorDownAttack01.png",	frames_attack1, fps_attack1,	w,	h);

	const int frames_attack2 = 6;
	const float fps_attack2 = frames_attack2 * 2.5;

	loadAnimation(Character::attack2, left, 	"data/Warrior/Left/Png/WarriorLeftAttack01.png",	frames_attack2, fps_attack2,	w,	h);
	loadAnimation(Character::attack2, right, 	"data/Warrior/Right/Png/WarriorRightAttack01.png",	frames_attack2, fps_attack2,	w,	h);
	loadAnimation(Character::attack2, up, 		"data/Warrior/Up/Png/WarriorUpAttack01.png",		frames_attack2, fps_attack2,	w,	h);
	loadAnimation(Character::attack2, down, 	"data/Warrior/Down/Png/WarriorDownAttack01.png",	frames_attack2, fps_attack2,	w,	h);

	const int frames_attack3 = 5;
	const float fps_attack3 = frames_attack3 * 2.5;

	loadAnimation(Character::attack3, left, 	"data/Warrior/Left/Png/WarriorLeftAttack01.png",	frames_attack3, fps_attack3,	w,	h);
	loadAnimation(Character::attack3, right, 	"data/Warrior/Right/Png/WarriorRightAttack01.png",	frames_attack3, fps_attack3,	w,	h);
	loadAnimation(Character::attack3, up, 		"data/Warrior/Up/Png/WarriorUpAttack01.png",		frames_attack3, fps_attack3,	w,	h);
	loadAnimation(Character::attack3, down, 	"data/Warrior/Down/Png/WarriorDownAttack01.png",	frames_attack3, fps_attack3,	w,	h);

	const int frames_hurt = 4;
	const float fps_hurt = frames_hurt * 2.5;

	loadAnimation(Character::hurt, left, 	"data/Warrior/Left/Png/WarriorLeftHurt.png",		frames_hurt, fps_hurt,	w,	h);
	loadAnimation(Character::hurt, right, 	"data/Warrior/Right/Png/WarriorRightHurt.png",		frames_hurt, fps_hurt,	w,	h);
	loadAnimation(Character::hurt, up, 		"data/Warrior/Up/Png/WarriorUpHurt.png",			frames_hurt, fps_hurt,	w,	h);
	loadAnimation(Character::hurt, down, 	"data/Warrior/Down/Png/WarriorDownHurt.png",		frames_hurt, fps_hurt,	w,	h);

	// zainicjowanie postaci na ekranie z domyślnymi ustawieniami
	init();
}