//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef BINANCEBEAST_WEBSOCKETTYPES_H
#define BINANCEBEAST_WEBSOCKETTYPES_H

#include <boost/variant.hpp>
#include <boost/beast.hpp>
#include "WebsocketResponse.h"

namespace bb::network::ws {
    class Stream;
};

using SharedStream = const std::shared_ptr<bb::network::ws::Stream>&;

namespace bb::network::ws {

    using CloseStreamCallback = std::function<void(SharedStream stream)>;
    using PongStreamCallback = std::function<void(SharedStream stream)>;
    using PingStreamCallback = std::function<void(SharedStream stream)>;
    using StreamCB = std::function<void(bool success, const std::string& data,SharedStream stream)>;
    using SendMessageCB = std::function<void(bool success)>;

    //Todo:: create other types (bytes, text, html, etc...)

}


#endif //BINANCEBEAST_WEBSOCKETTYPES_H
