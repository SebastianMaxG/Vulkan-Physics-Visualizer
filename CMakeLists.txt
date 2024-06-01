cmake_minimum_required(VERSION 3.10)

# Project Name
project(PhysicsVisualizer)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8 # Specify the version you need
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  bullet
  GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
  GIT_TAG        master  # or specify a version tag or commit hash
)

set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)  # Add this line

FetchContent_MakeAvailable(bullet)

# FetchContent_Declare(
#   hacd
#   GIT_REPOSITORY https://github.com/kmammou/v-hacd.git
#   GIT_TAG        master  # or specify a version tag or commit hash
# )
# 
# FetchContent_MakeAvailable(hacd)

# add_subdirectory(${hacd_SOURCE_DIR} ${CMAKE_BINARY_DIR}/hacd)


FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  # GIT_TAG        v1.85
)

FetchContent_MakeAvailable(imgui)

#set(IMGUI_SOURCE_FILES
#    "imgui.cpp"
#    "imgui_draw.cpp"
#    "imgui_widgets.cpp"
#    "imgui_tables.cpp"
#    "imgui_demo.cpp"
#    "backends/imgui_impl_glfw.cpp"
#    "backends/imgui_impl_vulkan.cpp"
#    "imgui.h"
#    "imconfig.h"
#    "imgui_internal.h"
#    "imstb_rectpack.h"
#    "imstb_textedit.h"
#    "imstb_truetype.h"
#    "backends/imgui_impl_glfw.h"
#    "backends/imgui_impl_vulkan.h"
#    )
#
#foreach(SOURCE IN ITEMS ${IMGUI_SOURCE_FILES})
#    list(APPEND ABSOLUTE_IMGUI_SOURCE_FILES "${imgui_SOURCE_DIR}/${SOURCE}")
#endforeach()
#
#add_library(imgui STATIC ${ABSOLUTE_IMGUI_SOURCE_FILES})
#link_directories(${CMAKE_BINARY_DIR})
#target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
#target_link_libraries(imgui PRIVATE ${Vulkan_LIBRARIES} glfw)

# Find the required packages
find_package(Vulkan REQUIRED)

# Include Directories
include_directories(${Vulkan_INCLUDE_DIRS})

include_directories("lib/tiny_obj_loader")
add_subdirectory(src)

# If using validation layers, copy the required JSON files (optional)
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_SOURCE_DIR}/path_to_json/VK_LAYER_KHRONOS_validation.json"
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
