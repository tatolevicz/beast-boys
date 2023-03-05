cmake_minimum_required(VERSION 3.12)
project(beast_boys VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        Network/SocketApi/Websocket.cpp
        Network/SocketApi/Websocket.h
        Network/SocketApi/Common/WebsocketTypes.h
        Network/SocketApi/Common/WebsocketResponse.cpp
        Network/SocketApi/Common/WebsocketResponse.h
        Network/SocketApi/Impl/WebsocketImpl.cpp
        Network/SocketApi/Impl/WebsocketImpl.h
        Network/SocketApi/Impl/SharedState.cpp
        Network/SocketApi/Impl/SharedState.h
        Network/Common/Logger.h
        Network/Common/CryptoHelper.h
        Network/SocketApi/Impl/Resolver.cpp
        Network/SocketApi/Impl/Resolver.h
        Network/SocketApi/Impl/Stream.cpp
        Network/SocketApi/Impl/Stream.h
        Network/SocketApi/Impl/Connector.cpp
        Network/SocketApi/Impl/Connector.h
        Network/SocketApi/Impl/SSLHandShaker.cpp
        Network/SocketApi/Impl/SSLHandShaker.h
        Network/SocketApi/Impl/TargetHandShaker.cpp
        Network/SocketApi/Impl/TargetHandShaker.h
        Network/SocketApi/Impl/Streamer.cpp
        Network/SocketApi/Impl/Streamer.h
        )

set(LIB_INCLUDES
        Network
        Network/Common
        Network/SocketApi
        Network/SocketApi/Common
        Network/SocketApi/Impl
        )

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

target_link_libraries( ${PROJECT_NAME} PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${LIB_INCLUDES}
        )

