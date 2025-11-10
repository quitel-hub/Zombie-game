cmake_minimum_required(VERSION 3.30)

project(Zombie-game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_DIR "C:/Users/quitel/CLionProjects/Zombie game/Zombie-game/libs/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)


add_library(GameLogic
        Game.cpp
        Entity.h
        Player.h
        Zombie.h
        Boss.h
        Weapon.h
        Sword.h
        Gun.h
        Inventory.h
        Map.h
        Game.h
        Container.h
        LocalizationManager.h
        Command.h

)


target_include_directories(GameLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(Zombie-game main.cpp)
target_link_libraries(GameLogic PRIVATE sfml-graphics sfml-window sfml-system)


set(SFML_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML-2.6.1/bin")


file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")

file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR})
target_link_libraries(Zombie-game PRIVATE GameLogic sfml-graphics sfml-window sfml-system)



