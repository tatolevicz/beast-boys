cmake_minimum_required(VERSION 3.12)
project(beast_boys VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF)


add_subdirectory(rapidjson)


include(beast_boys.cmake)

#CHANGE THIS TO YOUR VERSION/FOLDER #####################
set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl@1.1/1.1.1s)

add_library(${PROJECT_NAME} STATIC  ${BEAST_BOYS_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-dev)


find_package(OpenSSL REQUIRED)

if (${OPENSSL_FOUND})
    target_include_directories( ${PROJECT_NAME} PUBLIC
            ${OPENSSL_INCLUDE_DIR}
            )
else()
    message(FATAL_ERROR "OpenSSL not found please install it!")
endif()

find_package(Boost REQUIRED)
if (${Boost_FOUND})
    target_include_directories( ${PROJECT_NAME} PUBLIC
        ${Boost_INCLUDE_DIRS}
    )
else()
    message(FATAL_ERROR "Boost not found please install it!")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
        ${BEAST_BOYS_FOLDERS}
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
)


# SETUP EXAMPLES -----------------------------------
add_subdirectory(Examples/Streamer)
add_subdirectory(Examples/RestBinance)
#---------------------------------------------------


