cmake_minimum_required(VERSION 3.15)
project(CLASS_TEMPLATE LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

#IMGUI
set(IM_HEADER_DIR "${PROJECT_SOURCE_DIR}/../Dependencies/vendor/imgui")
file(GLOB IM_SOURCES "${PROJECT_SOURCE_DIR}/../Dependencies/vendor/imgui/*.cpp")


# Add the sources and headers to the project
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}")
add_executable(${PROJECT_NAME} ${IM_SOURCES}    DensityToMesh.cpp)
#add_executable(${PROJECT_NAME} ${IM_SOURCES}    3DFramebufferExample.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${IM_HEADER_DIR} )



#Vendor Header
include_directories("${PROJECT_SOURCE_DIR}/../Dependencies/vendor")

#OpenGL Tools
include_directories("${PROJECT_SOURCE_DIR}/OpenGLTools")
include_directories("${PROJECT_SOURCE_DIR}/OpenGLTools/Cameras")
include_directories("${PROJECT_SOURCE_DIR}/OpenGLTools/Collisions")
include_directories("${PROJECT_SOURCE_DIR}/OpenGLTools/Geometry")
include_directories("${PROJECT_SOURCE_DIR}/OpenGLTools/glmmath")
include_directories("${PROJECT_SOURCE_DIR}/CStringTools")

#Graphic Objects
include_directories("${PROJECT_SOURCE_DIR}/Objects")
include_directories("${PROJECT_SOURCE_DIR}/Objects/GraphicVector")
include_directories("${PROJECT_SOURCE_DIR}/Objects/InstancedGeometry")
include_directories("${PROJECT_SOURCE_DIR}/Objects/BoundingVolume")
include_directories("${PROJECT_SOURCE_DIR}/Objects/Frustrum")

#Extensions Marching Cubes
include_directories("${PROJECT_SOURCE_DIR}/MarchingCubes")
include_directories("${PROJECT_SOURCE_DIR}/MarchingCubes/primitives")


#Vol Parser
include_directories("${PROJECT_SOURCE_DIR}/VolParser")


#Libraries
#OpenGL GLFW3
include_directories("${PROJECT_SOURCE_DIR}/../Dependencies/GLEW/include")
include_directories("${PROJECT_SOURCE_DIR}/../Dependencies/GLFW/include")

#Library source code
target_link_libraries(${PROJECT_NAME} opengl32)
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/glfw3.dll")
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/glew32.dll")



set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<CONFIG:Debug>:Debug")
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION "true")
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_INCREMENTAL_LINK "true")