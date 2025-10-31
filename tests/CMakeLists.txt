
add_executable(runTests

        GameLogicTests.cpp
)

target_link_libraries(runTests PRIVATE gtest_main)


target_link_libraries(runTests PRIVATE GameLogic)


include(GoogleTest)
gtest_discover_tests(runTests)