
cmake_minimum_required(VERSION 3.12)

project(beast_boys VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF)

option(BUILD_BB_EXAMPLES "Build the tests" ON)
option(BUILD_BB_TESTS "Build the examples" ON)

# -- MGUTILS --------------------
add_subdirectory(mgutils)
# ----------------------------------------------

include(cmake/beast_boys.cmake)
include(cmake/generate_cacert_data.cmake)

add_library(${PROJECT_NAME} STATIC
    ${BEAST_BOYS_FILES}
)

set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(OpenSSL REQUIRED)
if (${OPENSSL_FOUND})
    target_include_directories( ${PROJECT_NAME} PUBLIC
        ${OPENSSL_INCLUDE_DIR}
    )
else()
    message(FATAL_ERROR "OpenSSL not found please install it!")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    ${BEAST_BOYS_FOLDERS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    mgutils
)

# SETUP EXAMPLES | TESTS -----------------------------------
if (${BUILD_BB_EXAMPLES})
    add_subdirectory(Examples/WebSocketStreamer)
    add_subdirectory(Examples/RestBinance)
    add_subdirectory(Examples/RawSocketMessenger)
    add_subdirectory(Examples/RawSock)
    add_subdirectory(Examples/RawSockServer)
endif()

if (${BUILD_BB_TESTS})
    add_subdirectory(Tests)
endif()
# ----------------------------------------------------------
