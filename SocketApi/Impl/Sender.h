//
// Created by Arthur Motelevicz on 06/03/23.
//

#ifndef BEAST_BOYS_MESSAGER_H
#define BEAST_BOYS_MESSAGER_H

#include "SharedState.h"

namespace bb {
namespace network {
namespace ws {

class Sender : public std::enable_shared_from_this<Sender> {
public:
    Sender(std::shared_ptr<Stream> stream);
    ~Sender();
    void send(const std::string& message, SendMessageCB cb);
    void onSend(boost::system::error_code ec, std::size_t bytes);
private:
    std::shared_ptr<Stream> _stream;
    network::ws::SendMessageCB _cb;
};

}
}
}
#endif //BEAST_BOYS_MESSAGER_H
