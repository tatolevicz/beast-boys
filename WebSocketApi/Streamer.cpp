//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "Streamer.h"
#include "WebsocketImpl.h"
#include "Stream.h"

namespace bb{

Streamer::Streamer(){
    _impl = std::make_shared<network::ws::WebsocketImpl>();
}

std::weak_ptr<network::ws::Stream>  Streamer::openStream(std::string baseUrl,
                                                         std::string port,
                                                         std::string endPoint,
                                                         bool usesSSL,
                                                         network::ws::StreamCB cb){

    return _impl->openStream(
            std::move(baseUrl),
            std::move(port),
            std::move(endPoint),
            usesSSL,
            std::move(cb));
}

}