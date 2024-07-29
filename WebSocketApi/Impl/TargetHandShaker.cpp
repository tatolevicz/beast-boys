//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "TargetHandShaker.h"
#include "Receiver.h"

namespace bb {
namespace network {
namespace ws {

TargetHandShaker::TargetHandShaker(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream):
_sharedState(std::move(state)),
_stream(std::move(stream))
{

}

void TargetHandShaker::onHandShake(boost::system::error_code ec) {

    if(ec){
        _stream->connectionAborted(ec);
        REPORT_ASIO_ERROR_(ec)
        return;
    }

    std::make_shared<Receiver>(std::move(_sharedState), std::move(_stream))->run();
}


void TargetHandShaker::run(){

    if(_stream->usesSSL()) {
        _stream->getSocketSSL().async_handshake(
                _stream->host(),
                _stream->target(),
                [self = shared_from_this()](boost::system::error_code ec) {
                    self->onHandShake(ec);
                }
        );

        return;
    }

    _stream->getSocket().async_handshake(
            _stream->host(),
            _stream->target(),
            [self = shared_from_this()](boost::system::error_code ec) {
                self->onHandShake(ec);
            }
    );

}

}
}
}