//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "Receiver.h"

namespace bb {
namespace network {
namespace ws {

Receiver::Receiver(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream):
_sharedState(std::move(state)),
_stream(std::move(stream))
{
    _sharedState->join(_stream.get());
}

Receiver::~Receiver(){
    _sharedState->leave(_stream.get());
}

void Receiver::onReceive(boost::system::error_code ec, std::size_t) {

    if (!ec || ec == boost::asio::error::eof) {
        auto msg =  boost::beast::buffers_to_string(_buffer.data());
        _stream->feedData(msg);
        _buffer.consume(_buffer.size());
        run();
        return;
    }

    _stream->connectionAborted(ec);
    REPORT_ASIO_ERROR_(ec)
}


void Receiver::run(){
    if(_stream->usesSSL()){
        _stream->getSocketSSL().async_read(_buffer,
                [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes){
                    self->onReceive(ec, bytes);
                }
        );
        return;
    }

    _stream->getSocket().async_read(_buffer,
          [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes){
              self->onReceive(ec, bytes);
          }
    );
}

}
}
}