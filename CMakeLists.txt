cmake_minimum_required(VERSION 3.12)

project(beast_boys VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    #set(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@1.1/1.1.1t)
    #set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl@1.1/1.1.1v)
    set(OPENSSL_ROOT_DIR OPENSSL_ROOT_DIR /Users/opala/Documents/EasyChart/openssl-mac)
    #set(OPENSSL_ROOT_DIR /Users/arthurmotelevicz/Documents/openssl_universal/openssl_x86_64)
    #set(OPENSSL_ROOT_DIR "C:/Users/Tato Levicz/Documents/MagiaTrade/openssl-mac")
    #set(OPENSSL_CRYPTO_LIBRARY "C:/Users/Tato Levicz/Documents/MagiaTrade/openssl-mac/lib")
    #set(OPENSSL_INCLUDE_DIR "C:/Users/Tato Levicz/Documents/MagiaTrade/openssl-mac/include")
elseif(WIN32)
    set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
    set(OPENSSL_CRYPTO_LIBRARY "C:/Program Files/OpenSSL-Win64/lib")
    set(OPENSSL_INCLUDE_DIR "C:/Program Files/OpenSSL-Win64/include")
    set(Boost_INCLUDE_DIR "C:/Users/Tato Levicz/Documents/MagiaTrade/boost_1_79_0")
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(BUILD_SHARED_LIBS OFF)

#Check if dependencies are installed
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external)
    message(FATAL_ERROR "Dependencies not installed! Please run ./install_dependencies.sh")
endif()
#-----------------------------------

# -- EXTERNAL subdirectories --------------------
#turn off rapidjson extra builds
option(RAPIDJSON_BUILD_DOC "Build rapidjson documentation." OFF )
option(RAPIDJSON_BUILD_EXAMPLES "Build rapidjson examples." OFF )
option(RAPIDJSON_BUILD_TESTS "Build rapidjson perftests and unittests." OFF )
option(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT "Build rapidjson with -march or -mcpu options" OFF )

add_subdirectory(external/rapidjson)
# ----------------------------------------------

include(beast_boys.cmake)

add_library(${PROJECT_NAME} STATIC  ${BEAST_BOYS_FILES} Common/cacert_data.h)

set(OPENSSL_USE_STATIC_LIBS TRUE)

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


