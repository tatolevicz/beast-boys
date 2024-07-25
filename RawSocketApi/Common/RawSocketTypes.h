//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef RAWSOCKETTYPES_H
#define RAWSOCKETTYPES_H

#include <boost/variant.hpp>
#include "RawSocketResponse.h"

namespace bb::network::rs {
    class Stream;
};

using SharedStream = const std::shared_ptr<bb::network::rs::Stream>&;

namespace bb::network::rs {

    using CloseStreamCallback = std::function<void(SharedStream stream)>;
    using PongStreamCallback = std::function<void(SharedStream stream)>;
    using PingStreamCallback = std::function<void(SharedStream stream)>;
    using StreamCB = std::function<void(bool success, const std::string& data,SharedStream stream)>;
    using SendMessageCB = std::function<void(bool success)>;

    //Todo:: create other types (bytes, text, html, etc...)

}


#endif //RAWSOCKETTYPES_H
