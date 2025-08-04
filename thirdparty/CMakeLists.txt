set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c
)

add_library(thirdparty ${SRC_FILES})
target_include_directories(
    thirdparty PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
