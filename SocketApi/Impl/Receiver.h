//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BINANCEBEAST_STREAMER_H
#define BINANCEBEAST_STREAMER_H

#include "SharedState.h"

namespace bb {
namespace network {
namespace ws {

class Receiver : public std::enable_shared_from_this<Receiver> {
public:
    Receiver(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream);
    ~Receiver();
    void run();
    void onReceive(boost::system::error_code ec, std::size_t bytes);
private:
    std::shared_ptr<Stream> _stream;
    std::shared_ptr<SharedState> _sharedState{nullptr};
    boost::beast::flat_buffer _buffer;
};

}
}
}

#endif //BINANCEBEAST_STREAMER_H
