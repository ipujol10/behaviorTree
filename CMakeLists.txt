cmake_minimum_required(VERSION 3.15)

set(LIBRARY_NAME BehaviorTree)
set(CMAKE_CXX_STANDARD 17)

project(${LIBRARY_NAME} VERSION 2.0)

set(SOURCE node.cpp control.cpp decorators.cpp)
foreach(item IN LISTS SOURCE)
    list(APPEND SOURCE src/${item})
    list(REMOVE_ITEM SOURCE ${item})
endforeach()

set(HEADER node.hpp control.hpp decorators.hpp)
foreach(item IN LISTS HEADER)
    list(APPEND HEADER include/BT/${item})
    list(REMOVE_ITEM HEADER ${item})
endforeach()

add_library(${LIBRARY_NAME} STATIC ${HEADER} ${SOURCE})

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/BT>
    $<INSTALL_INTERFACE:include/BT>
    )

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
FetchContent_MakeAvailable(googletest)

target_link_libraries(${LIBRARY_NAME} gtest_main)

add_subdirectory(test)

option(BUILD_TEST "Enable/Disable testing" ON)

if (BUILD_TEST)
    enable_testing()
    add_test(NAME BehaviorTreeTest COMMAND BehaviorTreeTest)
endif()

set(installable_lib ${LIBRARY_NAME})
install(TARGETS ${installable_lib} DESTINATION lib)
install(FILES ${HEADER} DESTINATION include/BT)
