cmake_minimum_required(VERSION 3.15)

set(LIBRARY_NAME BehaviorTree)
set(CMAKE_CXX_STANDARD 17)

project(${LIBRARY_NAME} VERSION 1.0)

add_library(${LIBRARY_NAME} STATIC
    include/BT/behaviorTree.hpp
    src/behaviorTree.cpp
    )

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
    )

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

add_subdirectory(test)

option(BUILD_TEST "Enable/Disable testing" ON)

if (BUILD_TEST)
    enable_testing()
    add_test(NAME BehaviorTreeTest COMMAND BehaviorTreeTest)
endif()
