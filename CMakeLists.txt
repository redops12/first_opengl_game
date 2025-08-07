cmake_minimum_required(VERSION 3.14)
project(first_game)

set(CMAKE_CXX_STANDARD 23)

include(CTest)

add_subdirectory(thirdparty)

# Source files
add_executable(first_game
    src/main.cpp
    src/Shape.cpp
)

# Include paths
target_link_libraries(first_game PUBLIC thirdparty)

# On Linux, you may also need to link these manually:
if (UNIX AND NOT APPLE)
    target_link_libraries(first_game PRIVATE dl m pthread X11 Xrandr Xi Xxf86vm Xcursor GL)
endif()
