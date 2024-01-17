project(behaviorTreeTest)

set(TEST_NAME BehaviorTreeTest)

if (BUILD_TEST)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        )
    FetchContent_MakeAvailable(googletest)

    add_executable(${TEST_NAME} nodeTest.cpp controlTest.cpp 
        decoratorsTest.cpp helper.hpp)
    target_link_libraries(${TEST_NAME} gtest_main BehaviorTree)
endif()
