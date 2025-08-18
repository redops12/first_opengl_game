set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c
)

add_subdirectory(glfw)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
add_subdirectory(assimp)

add_library(thirdparty ${SRC_FILES})
target_compile_options(thirdparty PRIVATE -Wno-error)
target_include_directories( thirdparty PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/
)

target_link_libraries(thirdparty PRIVATE glfw assimp)
