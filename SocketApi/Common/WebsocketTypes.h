//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef BINANCEBEAST_WEBSOCKETTYPES_H
#define BINANCEBEAST_WEBSOCKETTYPES_H

#include <boost/variant.hpp>
#include <boost/beast.hpp>
#include "WebsocketResponse.h"

namespace bb {
namespace network {
namespace ws {

using StreamCB = std::function<void(bool success, const std::string& data)>;
using SendMessageCB = std::function<void(bool success)>;
//Todo:: create other types (bytes, text, html, etc...)

}
}
}


#endif //BINANCEBEAST_WEBSOCKETTYPES_H
