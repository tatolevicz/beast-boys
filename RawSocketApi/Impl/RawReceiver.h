//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef RAW_STREAMER_H
#define RAW_STREAMER_H

#include "RawSharedState.h"

namespace bb {
namespace network {
namespace rs {

class Receiver : public std::enable_shared_from_this<Receiver> {
public:
    Receiver(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream);
    ~Receiver();
    void run();
    void onReceive(boost::system::error_code ec, std::size_t bytes);
private:
    std::shared_ptr<Stream> _stream;
    std::shared_ptr<SharedState> _sharedState{nullptr};
    boost::asio::streambuf _buffer;
};


}
}
}

#endif //RAW_STREAMER_H
