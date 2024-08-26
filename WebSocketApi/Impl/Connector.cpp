//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "Connector.h"
#include "SSLHandShaker.h"
#include "TargetHandShaker.h"
#include <openssl/ssl.h>

namespace bb {
namespace network {
namespace ws {

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

    _stream->setWatchControlMessages();

    if(_stream->usesSSL()){
        if (!SSL_set_tlsext_host_name(_stream->getSocketSSL().next_layer().native_handle(), _stream->host().c_str())) {
            auto error_code = boost::beast::error_code(
                    static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()
            );

            LOG_ERROR("SSL_set_tlsext_host_name error: " + error_code.message());
            return;
        }

        std::make_shared<SSLHandShaker>(std::move(_sharedState), std::move(_stream))->run();
        return;
    }

    std::make_shared<TargetHandShaker>(std::move(_sharedState), std::move(_stream))->run();
}

void Connector::run(const boost::asio::ip::tcp::resolver::results_type& results){
    if(_stream->usesSSL()) {
        boost::asio::async_connect(_stream->getSocketSSL().next_layer().next_layer(),
        results.begin(),
        results.end(),
        [self = shared_from_this()](
                boost::system::error_code ec,
                boost::asio::ip::tcp::resolver::iterator it) {
            self->onConnect(ec, std::move(it));
        });
        return;
    }

    boost::asio::async_connect(_stream->getSocket().next_layer(),
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