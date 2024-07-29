//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef RAW_SHAREDSTATE_H
#define RAW_SHAREDSTATE_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <unordered_set>
#include "Logger.h"
#include "RawStream.h"

namespace bb {
namespace network {
namespace rs {

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


#endif //RAW_SHAREDSTATE_H
