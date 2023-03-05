//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BINANCEBEAST_SHAREDSTATE_H
#define BINANCEBEAST_SHAREDSTATE_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <unordered_set>
#include "Logger.h"
#include "Stream.h"

namespace bb {
namespace network {
namespace ws {

    class SharedState : public std::enable_shared_from_this<SharedState> {
public:

    SharedState() = default;

    void join(Stream *);

    void leave(Stream *);

private:
    std::unordered_set<Stream *> _streams;
};

}
}
}


#endif //BINANCEBEAST_SHAREDSTATE_H
