# Zombie-game

A simple 2D zombie shooter, created in C++ using the SFML library.

## Requirements

To build and run the project, you will need:

* CMake: 3.30 or later.
* C++ Compiler: with C++20 standard support.
* SFML: 2.6.1 (graphics, window, and system components).

## Building and Running

1. Clone the repository:
```bash
git clone [your-repository-URL]
cd Zombie-game-main
```

2. Download SFML 2.6.1 from the [official website](https://www.sfml-dev.org/download.php).

3. Create a `libs` folder in the project root.

4. Unzip the SFML archive into this folder. Expected directory structure:
```
Zombie-game-main/
├── libs/
│ └── SFML-2.6.1/
│ ├── bin/
│ ├── include/
│ ├── lib/
│ └── ...
├── CMakeLists.txt
├── main.cpp
└── ...
```

5. **Important:** CMakeLists.txt specifies a hardcoded path to SFML:
```cmake
set(SFML_DIR) "C:/Users/quitel/CLionProjects/Zombie-game/libs/SFML-2.6.1/lib/cmake/SFML")
```
To ensure the project can be built on other computers, it is recommended to replace this path with a relative one using the `PROJECT_SOURCE_DIR` variable:
```cmake
# Make sure SFML is in the libs folder, as in step 4
set(SFML_DIR "${PROJECT_SOURCE_DIR}/libs/SFML-2.6.1/lib/cmake/SFML")
```

6. Create a build directory and build with CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

7. The executable (named `untitled23` according to `CMakeLists.txt`) will be located in the `build` folder.

## Running tests

The project uses Google Test (v1.14.0) for unit testing.

1. CMake will automatically configure and build the `runTests` target.
2. To run tests, run one of the commands from the `build` folder:

```bash
# Using CTest
ctest
```
Or
```bash
# Directly running the test executable
./runTests
```

## Technology Stack

* **Language:** C++20
* **Graphics Library:** SFML 2.6.1
* **Build System:** CMake 3.30+
* **Testing:** Google Test v1.14.0

## Project Structure

Below is the structure of the main files specified in `CMakeLists.txt`:A simple 2D zombie shooter, created in C++ using the SFML library.

## Requirements

To build and run the project, you will need:

* CMake: 3.30 or later.
* C++ Compiler: with C++20 standard support.
* SFML: 2.6.1 (graphics, window, and system components).

## Building and Running

1. Clone the repository:
```bash
git clone [your-repository-URL]
cd Zombie-game-main
```

2. Download SFML 2.6.1 from the [official website](https://www.sfml-dev.org/download.php).

3. Create a `libs` folder in the project root.

4. Unzip the SFML archive into this folder. Expected directory structure:
```
Zombie-game-main/
├── libs/
│ └── SFML-2.6.1/
│ ├── bin/
│ ├── include/
│ ├── lib/
│ └── ...
├── CMakeLists.txt
├── main.cpp
└── ...
```

5. **Important:** CMakeLists.txt specifies a hardcoded path to SFML:
```cmake
set(SFML_DIR) "C:/Users/quitel/CLionProjects/Zombie-game/libs/SFML-2.6.1/lib/cmake/SFML")
```
To ensure the project can be built on other computers, it is recommended to replace this path with a relative one using the `PROJECT_SOURCE_DIR` variable:
```cmake
# Make sure SFML is in the libs folder, as in step 4
set(SFML_DIR "${PROJECT_SOURCE_DIR}/libs/SFML-2.6.1/lib/cmake/SFML")
```

6. Create a build directory and build with CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

7. The executable (named `untitled23` according to `CMakeLists.txt`) will be located in the `build` folder.

## Running tests

The project uses Google Test (v1.14.0) for unit testing.

1. CMake will automatically configure and build the `runTests` target.
2. To run tests, run one of the commands from the `build` folder:

```bash
# Using CTest
ctest
```
Or
```bash
# Directly running the test executable
./runTests
```

## Technology Stack

* **Language:** C++20
* **Graphics Library:** SFML 2.6.1
* **Build System:** CMake 3.30+
* **Testing:** Google Test v1.14.0

## Project Structure

Below is the structure of the main files specified in `CMakeLists.txt`:
  cmake_minimum_required(VERSION 3.30)

project(Zombie-game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_DIR "your way to sfml")
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



enable_testing()


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)


add_subdirectory(tests)


Below is the structure of the main files specified in `CMakeLists.txt`(for unit-tests):

add_executable(runTests

        GameLogicTests.cpp
)

target_link_libraries(runTests PRIVATE gtest_main)


target_link_libraries(runTests PRIVATE GameLogic)


include(GoogleTest)
gtest_discover_tests(runTests)
