//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "RawConnector.h"
#include "RawReceiver.h"

namespace bb {
namespace network {
namespace rs {

Connector::Connector(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream):
_sharedState(std::move(state)),
_stream(std::move(stream))
{

}

void Connector::onConnect(boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator){

    if(ec){
        _stream->connectionAborted(ec);
        REPORT_ASIO_ERROR_(ec)
        return;
    }

    std::make_shared<Receiver>(std::move(_sharedState), std::move(_stream))->run();
}

void Connector::run(const boost::asio::ip::tcp::resolver::results_type& results){

    boost::asio::async_connect(_stream->getSocket(),
                               results.begin(),
                               results.end(),
    [self = shared_from_this()](
            boost::system::error_code ec,
            boost::asio::ip::tcp::resolver::iterator it) {
        self->onConnect(ec, std::move(it));
    });
}

}
}
}