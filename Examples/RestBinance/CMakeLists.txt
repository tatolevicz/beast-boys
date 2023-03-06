cmake_minimum_required(VERSION 3.12)
project(rest_binance VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        main.cpp
        ${BEAST_BOYS_FILES}
        )

if (${Boost_FOUND})
    target_include_directories(${PROJECT_NAME} PUBLIC
            ${Boost_INCLUDE_DIRS}
            )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${BEAST_BOYS_FOLDERS}
        )