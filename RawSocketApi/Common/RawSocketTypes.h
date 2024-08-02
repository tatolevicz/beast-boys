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

using RawSharedStream = const std::shared_ptr<bb::network::rs::Stream>&;

namespace bb::network::rs {

    using CloseStreamCallback = std::function<void(RawSharedStream stream)>;
    using PongStreamCallback = std::function<void(RawSharedStream stream)>;
    using PingStreamCallback = std::function<void(RawSharedStream stream)>;
    using StreamCB = std::function<void(bool success, const std::string& data, RawSharedStream stream)>;
    using StreamCB2 = std::function<void(bool success, const char* data, size_t length, RawSharedStream stream)>;
    using SendMessageCB = std::function<void(bool success)>;

    //Todo:: create other types (bytes, text, html, etc...)

}


#endif //RAWSOCKETTYPES_H
