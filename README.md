# Greetings Adventurer

## About project

### Gameplay
My intention is to create a open source game which will require from player logical thinking to win battles by controlling few characters turn after turn. In final perspective I want to give a player wonderful RPG experience by visiting many dungeons located on map with team of various characters and play there battles with enemies controlled by AI.
### Contribution
This game is quite big project. If you are C++ programmer with at least basic skills, your participation could help release first version of the game in reasonable time. Working on this game became opportunity to master my practical programming skills. The only condition to contribute on this project is to keep codebase maintainable.

All comments in code are written in Polish. I want give opportunity to join to my countrymates in the first place. Eventually everything will be translated to English. So far there is no other documentation so don't hesitate to contact me if you are interested in contribution and got any questions about code.
### Assets used in game
Sprites for characters used in project are distributed on [CC0 license](https://creativecommons.org/publicdomain/zero/1.0/) included in their directiories with all informations provided by author.
Some graphic are drawn by me but they are my doodles which are temporary. I also share them for for everybody with no restrictions if anybody wants would use them for any reasons.

If you are artist (graphical or musical) and you created or want to create assets to the game, I see this form of contribution in project as great too. Obviously you will be mentioned into credits.
## Installation
### Build using CMake
The easiest way to build project is build with CMake. The only requierements are C++ compiler, Git and CMake installed (whatever command or GUI version of it). Project uses [CMake SFML Project Template](https://github.com/SFML/cmake-sfml-project), in their readme you can find informations in case of troubles.
### Compile yourself on Linux
Get SFML developer version. Example installation on Debian-based distributions:

```sudo apt-get install libsfml-dev```

To get source code and compile project run following commands:

```git clone https://github.com/MaksSta/tactical-RPG.git```

```cd tactical-RPG```

```g++ -std=c++14 -g src/*.cpp src/GameEngine/*.cpp src/Characters/*.cpp src/Animations/*.cpp src/Animations/Actions/*.cpp src/Abilities/*.cpp src/UI/*.cpp src/AI/*.cpp src/AI/Actions/*.cpp -o tactical-RPG -lsfml-graphics -lsfml-window -lsfml-system```
### Compile yourself on other operating systems
[Download SFML](https://www.sfml-dev.org/download.php) for your OS and use [tutorial for your configuration](https://www.sfml-dev.org/tutorials/#getting-started).