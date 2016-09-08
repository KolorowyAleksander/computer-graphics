cmake_minimum_required(VERSION 3.5)
project(grafika)

#to squelch the casting from char* to const char*
set(CMAKE_CXX_FLAGS "-Wno-write-strings -std=c++11")

#set configuration file
set(SETTINGS settings.yaml)

#set source .cpp files to compile
set(SOURCES src/allmodels.h
            src/Camera.cpp
            src/Settings.cpp
            src/cube.cpp
            src/teapot.cpp
            src/lodepng.cpp
            src/shaderprogram.cpp
            src/model.cpp
            src/main_file.cpp)
#include_directories(src) #include headers for classes
#include_directories(glm) # include glm library headers

#find opengl main package end include headers if found
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_LIBS})
else()
    message("OpenGL not found")
endif()

#find GLEW (extension wrangler) and include headers if found
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
else()
    message("GLEW not found")
endif()

#find GLUT and include headers if found
find_package(GLUT REQUIRED)
if(GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIRS})
else()
    message("GLUT not found")
endif()

#find YAML and include headers if found
find_package(yaml-cpp REQUIRED)
if(YAMLCPP_FOUND)
    include_directories(${YAML_CPP_INCLUDE_DIR})
else()
    message("YAML not found")
endif()


#change module path to project to include FindGLFW.cmake
#find GLFW3 library and include headers if found
#set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(glfw3 3.2 REQUIRED)
if(GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
else()
    message("GLFW not found")
endif()

#set source .cpp files for compilation
set(ALL_FILES ${SOURCES} ${SHADERS} src/Maze.cpp src/Maze.h)

#add target for compilation
add_executable(grafika ${ALL_FILES})


#copy settings file to build directory
add_custom_command(TARGET grafika POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/${SETTINGS} $<TARGET_FILE_DIR:grafika>)

#copy glsl shader files to bin directory for runtime compilation
add_custom_command(TARGET grafika POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:grafika>)

#link all needed libraries against the object files
target_link_libraries(grafika ${OPENGL_LIBRARIES})
target_link_libraries(grafika ${GLEW_LIBRARIES})
target_link_libraries(grafika glfw)
target_link_libraries(grafika ${YAML_CPP_LIBRARIES})
