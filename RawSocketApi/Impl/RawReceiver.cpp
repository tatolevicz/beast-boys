//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "RawReceiver.h"

#define BUFFER_SIZE 2048

namespace bb {
namespace network {
namespace rs {

Receiver::Receiver(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream):
_sharedState(std::move(state)),
_stream(std::move(stream))
{
  _sharedState->join(_stream.get());
}

Receiver::~Receiver()
{
  _sharedState->leave(_stream.get());
}

void Receiver::onReceive(boost::system::error_code ec, std::size_t bytes)
{
  //return with no error handling if the stream was close by the control messages or by client
  if(_stream->wasClosedByServer() || _stream->wasClosedByClient())
    return;

  if (!ec)
  {
    // Extract the received data into a string
    std::istream is(&_buffer);

    if(_stream->getReadUntilDelimiter() != '\0')
    {
      std::string line;
      std::getline(is, line, _stream->getReadUntilDelimiter());
      _stream->feedData(line.data(), line.size());
      run();
      return;
    }

    _buffer.commit(bytes);

//    std::string msg((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    char msg[BUFFER_SIZE] = {0};

    // Ler os dados recebidos para o buffer de char[]
    is.read(msg, static_cast<long>(bytes));
    std::size_t msg_length = is.gcount(); // Número de bytes realmente lidos
//
    _stream->feedData(msg, msg_length);
    _buffer.consume(_buffer.size());

//    _stream->feedData(msg);
    run();
    return;
  }

  _stream->connectionAborted(ec);
  REPORT_ASIO_ERROR_(ec)
}

  void Receiver::run() {
    auto& socket = _stream->getSocket();
    if (socket.is_open())
    {
      if (_stream->getReadUntilDelimiter() == '\0')
      {
        // Se o delimitador não foi definido, faça uma leitura padrão
        socket.async_read_some(_buffer.prepare(BUFFER_SIZE),
        [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes) {
         self->onReceive(ec, bytes);
        });
      } else
      {
        // Use async_read_until com o delimitador definido
        boost::asio::async_read_until(socket, _buffer, _stream->getReadUntilDelimiter(),
        [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
        {
          self->onReceive(ec, bytes);
        });
      }
    }
  }

//void Receiver::run(){
//  auto &socket = _stream->getSocket();
//  if (socket.is_open())
//  {
//    socket.async_read_some(_buffer.prepare(BUFFER_SIZE),
//    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
//    {
//     self->onReceive(ec, bytes);
//    });
//  }
//}

}
}
}