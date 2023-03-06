set(BEAST_BOYS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Websocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Websocket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Common/WebsocketTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Common/WebsocketResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Common/WebsocketResponse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/WebsocketImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/WebsocketImpl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/SharedState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/SharedState.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Common/Logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Common/CryptoHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Resolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Resolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Stream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Connector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Connector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/SSLHandShaker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/SSLHandShaker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/TargetHandShaker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/TargetHandShaker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Streamer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl/Streamer.h
)

set(BEAST_BOYS_FOLDERS
     ${CMAKE_CURRENT_SOURCE_DIR}/Common
     ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi
     ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Common
     ${CMAKE_CURRENT_SOURCE_DIR}/SocketApi/Impl
)
