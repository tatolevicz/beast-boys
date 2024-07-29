//
// Created by Arthur Motelevicz on 06/03/23.
//

#ifndef RAW_SENDER_H
#define RAW_SENDER_H

#include "RawSharedState.h"

namespace bb {
namespace network {
namespace rs {

class Sender : public std::enable_shared_from_this<Sender> {
public:
    Sender(std::shared_ptr<Stream> stream);
    ~Sender();
    void send(const std::string& message, SendMessageCB cb);
    void onSend(boost::system::error_code ec, std::size_t bytes);
private:
    std::shared_ptr<Stream> _stream;
    network::rs::SendMessageCB _cb;
};

}
}
}
#endif //RAW_SENDER_H
