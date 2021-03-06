cmake_minimum_required(VERSION 3.5)
project(grafika)

#to squelch the casting from char* to const char*
set(CMAKE_CXX_FLAGS "-Wno-write-strings -std=c++11")

#set configuration file
set(SETTINGS settings.yaml)

#set source .cpp files to compile
set(SOURCES src/Maze.cpp
            src/Maze.h
            src/Camera.cpp
            src/Settings.cpp
            src/ShaderProgram.cpp
        src/main.cpp)

set(SHADERS shaders/wallFragmentShader.glsl
        shaders/wallVertexShader.glsl)

set(RESOURCES res/brickwall.png)

set(LIBS lib/lodepng.h
         lib/lodepng.cpp)

#find opengl main package end include headers if found
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_LIBS})
else ()
    message("OpenGL not found")
endif ()

#find GLEW (extension wrangler) and include headers if found
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
else ()
    message("GLEW not found")
endif ()

#find GLUT and include headers if found
find_package(GLUT REQUIRED)
if (GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIRS})
else ()
    message("GLUT not found")
endif ()

#find YAML and include headers if found
find_package(yaml-cpp REQUIRED)
if (YAMLCPP_FOUND)
    include_directories(${YAML_CPP_INCLUDE_DIR})
else ()
    message("YAML not found")
endif ()

#find GLFW3 library and include headers if found
find_package(glfw3 3.2 REQUIRED)
if (GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
else ()
    message("GLFW not found")
endif ()

#set source .cpp files for compilation
set(ALL_FILES ${SOURCES} ${LIBS} src/Cube.cpp src/Cube.h src/Main.cpp src/Main.h)

#add target for compilation
add_executable(grafika ${ALL_FILES})


add_custom_command(TARGET grafika PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/${SETTINGS}
                                                                 $<TARGET_FILE_DIR:grafika>
                   COMMENT "Copying settings file to build target directory")

add_custom_command(TARGET grafika PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders
                                                              $<TARGET_FILE_DIR:grafika>
                   COMMENT "Copying shaders to build target directory: $<TARGET_FILE_DIR:grafika>")

add_custom_command(TARGET grafika PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res
                                                              $<TARGET_FILE_DIR:grafika>
                   COMMENT "Copying resources to build target directory")

#link all needed libraries against the object files
target_link_libraries(grafika ${OPENGL_LIBRARIES})
target_link_libraries(grafika ${GLEW_LIBRARIES})
target_link_libraries(grafika glfw)
target_link_libraries(grafika ${YAML_CPP_LIBRARIES})