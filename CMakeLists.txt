cmake_minimum_required(VERSION 3.18)

project("opengl-playground")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -mwindows -municode")

find_package(OpenGL REQUIRED) # PATHS "P:\\Downloads\\cpp-libraries\\portaudio\\build\\cmake\\portaudio")

if(OpenGL_FOUND)
    message(STATUS "YES")
else()
    message(STATUS "NO")
endif()

add_executable(${PROJECT_NAME}
    "src/main_v2.cpp"
)

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
