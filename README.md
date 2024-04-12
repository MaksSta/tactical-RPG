# Greetings Adventurer

## About project

### Gameplay
My intention is to create a open source game which will require from player logical thinking to win battles by controlling few characters turn after turn (similar to games like HoMM or [Dofus](https://www.dofus.com/)). In final perspective I want to give a player wonderful RPG experience by visiting many dungeons located on map with team of various characters. They will move through dungeon rooms meeting there enemies controlled by AI (similar to game [Darkest Dungeon](https://store.steampowered.com/app/262060/darkest_dungeon/)). 
### Release
The goal of first release is to give ability to play sample battle. I came with idea of [markdown file with feature list](Before%20first%20release%20TODO.md) which are left to achieve this goal.
### Contribution
This game is quite big project. If you are C++ programmer with at least basic skills, your participation could help release first version of the game in reasonable time. Working on this game became opportunity to master my practical programming skills. The only condition to contribute on this project is to keep codebase maintainable.

All comments in code are written in Polish. I want give opportunity to join to my countrymates in the first place. Eventually everything will be translated to English. So far there is no other documentation so don't hesitate to contact me if you are interested in contribution and got any questions about code.
### Assets used in game
Sprites for characters used in project are distributed on [CC0 license](https://creativecommons.org/publicdomain/zero/1.0/) included in their directiories with all informations provided by author.
Some graphic are drawn by me but they are my doodles which are temporary. I also share them for for everybody with no restrictions if anybody wants would use them for any reasons.

If you are artist (graphical or musical) and you created or want to create assets to the game, I see this form of contribution in project as great too. Obviously you will be mentioned into credits.
## Installation
### Requirements
I haven't prepared CMake file yet. To compile code you need to get SFML library on your computer and use for it at least C++14.
### Linux
To get SFML (used 2d graphics library) on Debian-based distributions:

```sudo apt-get install libsfml-dev```

To get source code and compile project run following commands:

```git clone https://github.com/MaksSta/tactical-RPG.git```

```cd tactical-RPG```

```g++ -std=c++14 -g src/*.cpp src/GameEngine/*.cpp src/Characters/*.cpp src/Animations/*.cpp src/Animations/Actions/*.cpp src/Abilities/*.cpp src/UI/*.cpp -o tactical-RPG -lsfml-graphics -lsfml-window -lsfml-system```
### Others Operating systems
Download SFML for your OS from https://www.sfml-dev.org/download/sfml/2.6.1/

To get the source code:

```git clone https://github.com/MaksSta/tactical-RPG.git```

Find your configuration at https://www.sfml-dev.org/tutorials/2.6/#getting-started

Add all \*.cpp files from directory tactical-RPG/src/ and all its subdirectories
compile project using at least c++14