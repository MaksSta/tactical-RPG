cmake_minimum_required(VERSION 3.16)
project(RPG LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(SOURCE_FILES
	src/Abilities/Ability.cpp
	src/Abilities/Attack.cpp
	src/Animations/Actions/Move.cpp
	src/Animations/Death.cpp
	src/Animations/Disappear.cpp
	src/Animations/Hurt.cpp
	src/Animations/Manager.cpp
	src/Animations/MoveBy.cpp
	src/Animations/OnTexture.cpp
	src/Characters/Character.cpp
	src/Characters/CharacterOnBoard.cpp
	src/Characters/GoblinSlinger.cpp
	src/Characters/HpBar.cpp
	src/Characters/Sorceress.cpp
	src/Characters/Warrior.cpp
	src/GameEngine/BattleQueue.cpp
	src/GameEngine/Camera.cpp
	src/GameEngine/Field.cpp
	src/GameEngine/FieldSet.cpp
	src/GameEngine/Game.cpp
	src/GameEngine/Level.cpp
	src/GameEngine/Pathfinder.cpp
	src/GameEngine/Range.cpp
	src/GameEngine/Road.cpp
	src/main.cpp
	src/Program.cpp
	src/UI/Button.cpp
	src/UI/TextBox.cpp
	src/UI/Textfield.cpp
	src/UI/UI.cpp
	src/GameEngine/FullBoard.cpp
	src/GameEngine/ActiveBoard.cpp
	src/AI/Core.cpp
	src/AI/Decision.cpp
	src/AI/Actions/Action.cpp
	src/AI/Actions/Move.cpp
	src/AI/Actions/Attack.cpp
	src/AI/Actions/FinishTurn.cpp
	)

SET(HEADER_FILES
	src/Abilities/Ability.h
	src/Abilities/Attack.h
	src/Animations/Actions/Move.h
	src/Animations/Animation.h
	src/Animations/Death.h
	src/Animations/Disappear.h
	src/Animations/Hurt.h
	src/Animations/Manager.h
	src/Animations/MoveBy.h
	src/Animations/OnTexture.h
	src/Characters/Character.h
	src/Characters/CharacterOnBoard.h
	src/Characters/GoblinSlinger.h
	src/Characters/HpBar.h
	src/Characters/Sorceress.h
	src/Characters/Warrior.h
	src/GameEngine/BattleQueue.h
	src/GameEngine/Camera.h
	src/GameEngine/Field.h
	src/GameEngine/FieldSet.h
	src/GameEngine/Game.h
	src/GameEngine/Level.h
	src/GameEngine/Pathfinder.h
	src/GameEngine/Range.h
	src/GameEngine/Road.h
	src/Global.h
	src/Program.h
	src/UI/Button.h
	src/UI/TextBox.h
	src/UI/Textfield.h
	src/UI/UI.h
	src/GameEngine/FullBoard.h
	src/GameEngine/ActiveBoard.h
	src/AI/Core.h
	src/AI/Decision.h
	src/AI/Actions/Action.h
	src/AI/Actions/Move.h
	src/AI/Actions/Attack.h
	src/AI/Actions/FinishTurn.h
	)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(RPG PRIVATE sfml-graphics)
target_compile_features(RPG PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET RPG
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:RPG>
        VERBATIM)
endif()
