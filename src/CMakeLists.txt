# Source files
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/Shaders")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

set(SOURCES
    "main.cpp"
    "FirstApp.cpp"
    "FirstApp.h"
    "ProjectDevice.cpp"
    "ProjectDevice.h"
    "ProjectPipeline.cpp"
    "ProjectPipeline.h"
    "ProjectSwapChain.cpp"
    "ProjectSwapChain.h"
    "ProjectWindow.cpp"
    "ProjectWindow.h"
    "ProjectModel.cpp"
    "ProjectModel.h"
    "ProjectGameObject.h" 
    "ProjectRenderer.h" 
    "ProjectRenderer.cpp" 
    "SimpleRenderSystem.h"
    "SimpleRenderSystem.cpp"
    "ProjectCamera.h" 
    "ProjectCamera.cpp" 
    "ProjectInputController.h" 
    "ProjectInputController.cpp"
    "ProjectUtils.h" 
    "ProjectGameObject.cpp"
    )

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${GLSL_SOURCE_FILES} )
add_dependencies(${PROJECT_NAME} Shaders)

set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)

# Link libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} glfw)

# Include Bullet Physics 
target_include_directories(${PROJECT_NAME} PRIVATE ${bullet_SOURCE_DIR}/src)

# Include HACD
target_include_directories(${PROJECT_NAME} PRIVATE ${bullet_SOURCE_DIR}/Extras)

target_link_libraries(${PROJECT_NAME} PRIVATE BulletDynamics BulletCollision LinearMath HACD)

# Include ImGui

# target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# Include HACD
# target_include_directories(${PROJECT_NAME} PRIVATE ${hacd_SOURCE_DIR}/src/VHACD_Lib/public)

# target_link_libraries(${PROJECT_NAME} PRIVATE VHACD_LIB)

# target_link_libraries(${PROJECT_NAME} PRIVATE ${hacd_SOURCE_DIR}/lib/Release/VHACD_LIB.lib)
