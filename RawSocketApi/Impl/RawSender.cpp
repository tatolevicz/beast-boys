//
// Created by Arthur Motelevicz on 06/03/23.
//

#include "RawSender.h"

namespace bb {
namespace network {
namespace rs {

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

  //  _stream->getSocket().async_write(boost::asio::buffer(message),
//  [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes){
//      self->onSend(ec, bytes);
//  });

  auto &socket = _stream->getSocket();
  if (socket.is_open())
  {
    boost::asio::async_write(socket, boost::asio::buffer(message),
    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred)
    {
      self->onSend(ec, bytes_transferred);
    });
}
}

}
}
}