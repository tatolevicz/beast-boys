//
// Created by Arthur Motelevicz on 06/03/23.
//

#include "Sender.h"

namespace bb {
namespace network {
namespace ws {

Sender::Sender(std::shared_ptr<Stream> stream):
        _stream(std::move(stream))
{
}

Sender::~Sender(){
}

void Sender::onSend(boost::system::error_code ec, std::size_t) {
    if(!ec) {
        if(_cb)
            _cb(true);

        return;
    }

    REPORT_ASIO_ERROR_(ec)

    if(_cb)
        _cb(false);
}

void Sender::send(const std::string& message, SendMessageCB cb){
    _cb = std::move(cb);
    if(_stream->usesSSL()){
        _stream->getSocketSSL().async_write(boost::asio::buffer(message),
        [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes) {
           self->onSend(ec, bytes);
        });
        return;
    }

    _stream->getSocket().async_write(boost::asio::buffer(message),
    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes){
        self->onSend(ec, bytes);
    });
}

}
}
}