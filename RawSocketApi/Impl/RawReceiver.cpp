//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "RawReceiver.h"

namespace bb {
namespace network {
namespace rs {

Receiver::Receiver(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream):
_sharedState(std::move(state)),
_stream(std::move(stream))
{
    _sharedState->join(_stream.get());
}

Receiver::~Receiver(){
    _sharedState->leave(_stream.get());
}

void Receiver::onReceive(boost::system::error_code ec, std::size_t)
{
    //return with no error handling if the stream was close by the control messages or by client
    if(_stream->wasClosedByServer() || _stream->wasClosedByClient())
      return;

    if (!ec) {
      // Extract the received data into a string
      std::istream is(&_buffer);
      std::string msg((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
      _stream->feedData(msg);
      _buffer.consume(_buffer.size());
      run();
      return;
    }

    _stream->connectionAborted(ec);
    REPORT_ASIO_ERROR_(ec)
}


void Receiver::run(){
  auto &socket = _stream->getSocket();
  if (socket.is_open())
  {
    boost::asio::async_read(socket,_buffer,
    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
    {
      self->onReceive(ec, bytes);
    });
  }
}

}
}
}