cmake_minimum_required(VERSION 3.12)
project(raw-server VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    beast_boys
)