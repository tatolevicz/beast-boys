//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "Websocket.h"
#include "WebsocketImpl.h"
#include "Stream.h"

namespace bb{
namespace network{
namespace ws{

Websocket::Websocket(){
    _impl = std::make_shared<WebsocketImpl>();
}

std::weak_ptr<Stream>  Websocket::openStream(std::string baseUrl,
                                             std::string port,
                                             std::string endPoint,
                                             bool usesSSL,
                                             StreamCB cb){

    return _impl->openStream(
            std::move(baseUrl),
            std::move(port),
            std::move(endPoint),
            usesSSL,
            std::move(cb));
}

}
}
}