# Skrypt do generowania klasy postaci
# Skonfigurowany pod ładowanie ścieżek grafik stworzonych przez twórcę Foozle

# Jako argument należy podać nazwę Postaci. Zostaną utworzone pliki:
# - Header file
# - Source file

# Przed dodaniem postaci do gry należy samodzielnie dodać dyrektywę pliku nagłówkego do Game.h:
# #include "../Characters/{nazwa postaci}.h"

#import os
import sys

classname = sys.argv[1]

# Utworzenie pliku .h

header_file = open('../src/Characters/'+classname+'.h', 'w')
header_file.write(
"""#ifndef """+ classname.upper() +"""_H_
#define """+ classname.upper() +"""_H_

#include "CharacterOnBoard.h"

class """+classname+""" : public CharacterOnBoard {
public:
  """+classname+"""(sf::Vector2i start_coords);
};

#endif /* """ + classname.upper() + """_H_ */"""
)

header_file.close

# Utworzenie pliku .cpp

directions = ["Left", "Right" , "Up", "Down"]
actions = ["idle", "move", "hurt", "death", "attack1"]

source_file = open('../src/Characters/'+classname+'.cpp', 'w')
source_file.write(
"""#include \""""+classname+""".h"

"""+classname+"""::"""+classname+"""(sf::Vector2i start_coords)
  :
  CharacterOnBoard(start_coords,
                   \""""+classname+"""\",
                   Team::player, // TODO można w tym miejscu zmienić na Team::enemy
                   12,
                   {-2, -2},
                   2)
{
  finish_turn_button.desc = "Finish turn";
  finish_turn_button.img_file_path = "data/UI/finish_turn.png";

  // TODO należy samodzielnie wprowadzić ścieżkę do grafiki ataku w UI i nazwę
  Button_data button_attack1_data;
  button_attack1_data.desc = "";
  button_attack1_data.img_file_path = "data/UI/"""+classname+"""/.png";
  button_attack1_data.id_name = "";

  button_data_for_attack[Activity::attack1] = button_attack1_data;

  // TODO zmienić ustawienia ataku, kolejny argumenty po Activity::attack1 to: rodzaj ataku, koszt akcji, min range, max range, min dmg, max dmg
  attack.push_back({Activity::attack1, Attack::CallType::targetable, 3, 1, 4, 3, 6});

  // TODO w celu dodania kolejnych ataków należy skopiować cały powyższy fragment zmieniając attack1 na attack2 itd.
  // należy również skopiować fragment na końcu pliku w którym ładowana jest grafika postaci do ataku

  const int w = 48;
  const int h = 48;"""
)

src_for_action = {
    "idle" : "Idle",
    "move" : "Run", # ewentualnie podmienić na Walk przed wygenerowaniem
    "hurt" : "Hurt",
    "death" : "Death",
    "attack1" : "Attack01"
}

for action in actions:
     source_file.write("""\n
  // TODO ilość klatek została wpisana jako 6, należy sprawdzić samemu ile jest i wpisać
  const int frames_"""+action+""" = 6;
  const float fps_"""+action+""" = frames_"""+action+""" * 1.5;\n\n""")
     for d in directions:
	     source_file.write("  loadAnimation(Activity::"+action+", "+d.lower()+", \"data/"+classname + "/" + d + "/Png/" + classname + d + src_for_action[action] +".png\", frames_idle, fps_idle, w, h);\n"
  )

source_file.write('\n\n  //zainicjowanie postaci na ekranie z domyślnymi ustawieniami\n  init();\n}')

print('Utworzono postać '+classname)
