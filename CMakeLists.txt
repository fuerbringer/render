cmake_minimum_required(VERSION 3.28.3)

project(render VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2
find_package(SDL2 REQUIRED)

include_directories(include)

# TARGET: library with core functionality
add_library(render source/framebuffer.cpp source/vector.cpp source/object.cpp source/render.cpp source/sdlplatform.cpp)
target_compile_options(render PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(render PRIVATE SDL2::SDL2)
target_include_directories(render PRIVATE ${SDL2_INCLUDE_DIRS})

# TARGET: app frontend to link against core lib
add_executable(app source/main.cpp)
target_link_libraries(app PRIVATE render)

# TARGET: unit tests
enable_testing()
add_executable(testapp test/test.cpp)
# Googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(testapp GTest::gtest_main)
target_link_libraries(testapp render)
include(GoogleTest)
gtest_discover_tests(testapp)
