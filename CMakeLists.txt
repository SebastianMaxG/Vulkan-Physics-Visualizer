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
