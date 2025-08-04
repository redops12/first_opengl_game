cmake_minimum_required(VERSION 3.14)
project(first_game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -fno-omit-frame-pointer")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
add_compile_options("-Wall" "-Wextra")

include(CTest)

add_subdirectory(thirdparty)

# Source files
add_executable(first_game
    src/main.cpp
)

# Link glfw
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(first_game PRIVATE glfw)

# Include paths
target_link_libraries(first_game PRIVATE thirdparty)


# On Linux, you may also need to link these manually:
if (UNIX AND NOT APPLE)
    target_link_libraries(first_game PRIVATE dl m pthread X11 Xrandr Xi Xxf86vm Xcursor GL)
endif()
