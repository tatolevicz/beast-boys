//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "SSLHandShaker.h"
#include "TargetHandShaker.h"

namespace bb {
namespace network {
namespace ws {

SSLHandShaker::SSLHandShaker(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream):
_sharedState(std::move(state)),
_stream(std::move(stream))
{

}

void SSLHandShaker::onHandShake(boost::system::error_code ec) {
    if(ec){
        _stream->connectionAborted(ec);
        REPORT_ASIO_ERROR_(ec)
        return;
    }
    std::make_shared<TargetHandShaker>(std::move(_sharedState), std::move(_stream))->run();
}


void SSLHandShaker::run(){
    _stream->getSocketSSL().next_layer().async_handshake(
            boost::asio::ssl::stream_base::client,
            [self = shared_from_this()](boost::system::error_code ec){
                self->onHandShake(ec);
            }
    );
}

}
}
}