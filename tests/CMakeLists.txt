
add_executable(runTests

        GameLogicTests.cpp
)

target_link_libraries(runTests PRIVATE gtest_main)


target_link_libraries(runTests PRIVATE GameLogic)


include(GoogleTest)
gtest_discover_tests(runTests)


# --- Google Benchmark Setup ---
include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)


add_executable(game_benchmarks
        tests/Benchmarks.cpp

)

target_link_libraries(game_benchmarks PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
        sfml-graphics sfml-window sfml-system sfml-audio
)