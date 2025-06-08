add_executable (BezierTesselation "BezierTesselation.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BezierTesselation PROPERTY CXX_STANDARD 20)
endif()

set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/vendor/imgui")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

set(RES_DIR "${PROJECT_SOURCE_DIR}/res")
file(GLOB_RECURSE RES_FILES "${RES_DIR}/*")

target_sources(BezierTesselation PRIVATE ${IMGUI_SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glew/build/cmake glew)
add_subdirectory(${PROJECT_SOURCE_DIR}/math3d math3d)

include_directories(${PROJECT_SOURCE_DIR}/vendor/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/imgui)

target_link_libraries(BezierTesselation PRIVATE glfw glew_s opengl32 math3d)

add_custom_command(
    TARGET BezierTesselation PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:BezierTesselation>/res"
    COMMENT "Copying shaders to output directory"
)

