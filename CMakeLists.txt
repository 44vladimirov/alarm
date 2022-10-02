cmake_minimum_required(VERSION 3.12)
project(alarm)

add_subdirectory(cppsndio)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/Alarm.hpp
        src/CmdArgs.hpp
        src/CmdArgs.cpp
        src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        cppsndio
)
