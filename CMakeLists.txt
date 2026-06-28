cmake_minimum_required(VERSION 3.10)

project(Lab10BoidsStart LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(Lab10Boids
    src/main.cpp
    src/App.cpp
    src/Flock.cpp
    src/Renderer.cpp
)

target_include_directories(Lab10Boids PRIVATE
    include
    ${GLUT_INCLUDE_DIRS}
)

if(TARGET OpenGL::GL AND TARGET GLUT::GLUT)
    target_link_libraries(Lab10Boids PRIVATE OpenGL::GL GLUT::GLUT)
else()
    target_link_libraries(Lab10Boids PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
endif()

if(WIN32)
    target_link_libraries(Lab10Boids PRIVATE glu32)
endif()
