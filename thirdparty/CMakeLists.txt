set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c
)

add_subdirectory(glfw)

add_library(thirdparty ${SRC_FILES})
target_include_directories(
    thirdparty PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
    thirdparty PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/
)

target_link_libraries(thirdparty PUBLIC glfw)

# Remove top level cmake options
get_target_property(THIRDPARTY_COMPILE_OPTIONS thirdparty COMPILE_OPTIONS)
list(REMOVE_ITEM THIRDPARTY_COMPILE_OPTIONS "-Wunused") # Remove a specific flag
set_target_properties(thirdparty PROPERTIES COMPILE_OPTIONS "${THIRDPARTY_COMPILE_OPTIONS}")
