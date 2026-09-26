FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

add_executable(interpreter_tests InterpreterTests.cpp)
target_link_libraries(interpreter_tests PRIVATE GTest::gtest_main interpreter_lib)

include(GoogleTest)
gtest_discover_tests(interpreter_tests)
