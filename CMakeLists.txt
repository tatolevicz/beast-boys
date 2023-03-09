cmake_minimum_required(VERSION 3.12)

project(beast_boys VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@1.1/1.1.1t)

set(BUILD_SHARED_LIBS OFF)

#Install dependencies shell command
execute_process(COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/install_dependencies_on_build.sh" RESULT_VARIABLE added_dependencies)

if(${added_dependencies} EQUAL 0)
    message("Dependencies added with success!")
else()
    message(FATAL_ERROR "Error trying to install dependencies: ${added_dependencies}")
endif()

# -- EXTERNAL subdirectories --------------------

#turn off rapidjson extra builds
option(RAPIDJSON_BUILD_DOC "Build rapidjson documentation." OFF )
option(RAPIDJSON_BUILD_EXAMPLES "Build rapidjson examples." OFF )
option(RAPIDJSON_BUILD_TESTS "Build rapidjson perftests and unittests." OFF )
option(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT "Build rapidjson with -march or -mcpu options" OFF )

add_subdirectory(external/rapidjson)
# ----------------------------------------------

include(beast_boys.cmake)

add_library(${PROJECT_NAME} STATIC  ${BEAST_BOYS_FILES})

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


