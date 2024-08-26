cmake_minimum_required(VERSION 3.12)
project(bb-tests VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    TestHelpers.cpp
    TestHelpers.h
    cases/raw_connections.cpp
    cases/raw_messaging.cpp
    cases/ws_connections.cpp
    cases/ws_messaging.cpp
    cases/restapi.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE beast_boys)

target_include_directories(${PROJECT_NAME} PUBLIC
    ./
)