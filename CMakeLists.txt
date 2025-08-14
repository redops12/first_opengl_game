cmake_minimum_required(VERSION 3.14)
project(first_game)

set(CMAKE_CXX_STANDARD 23)

include(CTest)

add_subdirectory(thirdparty)

# Source files
add_executable(dvd
    src/dvd.cpp
    src/Shape.cpp
    src/Shader.cpp
    src/image_loader.cpp
)

# Source files
add_executable(dvd_3d_1
    src/dvd_3d_1.cpp
    src/Shape.cpp
    src/Shader.cpp
    src/image_loader.cpp
)

# Source files
add_executable(first_game
    src/main.cpp
    src/Shape.cpp
    src/Shader.cpp
    src/image_loader.cpp
    src/obj_loader.cpp
)

# Include paths
target_link_libraries(dvd PUBLIC thirdparty)
target_link_libraries(dvd_3d_1 PUBLIC thirdparty)
target_link_libraries(first_game PUBLIC thirdparty)

# On Linux, you may also need to link these manually:
if (UNIX AND NOT APPLE)
    target_link_libraries(dvd PRIVATE dl m pthread X11 Xrandr Xi Xxf86vm Xcursor GL)
    target_link_libraries(dvd_3d_1 PRIVATE dl m pthread X11 Xrandr Xi Xxf86vm Xcursor GL)
    target_link_libraries(first_game PRIVATE dl m pthread X11 Xrandr Xi Xxf86vm Xcursor GL)
endif()
