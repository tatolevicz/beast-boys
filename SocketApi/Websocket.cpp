//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "Websocket.h"
#include "WebsocketImpl.h"

namespace bb{
namespace network{
namespace ws{

Websocket::Websocket(){
    _impl = std::make_shared<WebsocketImpl>();
}

int Websocket::openStream(std::string baseUrl,
                          std::string port,
                          std::string endPoint,
                          bool usesSSL){

//    _impl->openStream(
//            std::move(baseUrl),
//            std::move(port),
//            std::move(endPoint),
//            usesSSL,
//            nullptr);

    return 0;

}

bool Websocket::hasWork(){
    return _impl->hasWork();
}

void Websocket::closeStream(const int id){

}


}
}
}