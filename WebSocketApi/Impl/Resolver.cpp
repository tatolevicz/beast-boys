//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "Resolver.h"
#include "Connector.h"

namespace bb {
namespace network {
namespace ws {

Resolver::Resolver(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream,  boost::asio::io_context &ioc):
_sharedState(std::move(state)),
_stream(std::move(stream)),
_resolver(ioc)
{

}

void Resolver::onResolve(boost::system::error_code ec, const boost::asio::ip::tcp::resolver::results_type& res){
    if(ec)
    {
      _stream->connectionAborted(ec);
      RETURN_IF_ASIO_ERROR_(ec)
    }

    std::make_shared<Connector>(std::move(_sharedState), std::move(_stream))->run(res);
}


void Resolver::run(){
    _resolver.async_resolve(_stream->host(),
                            _stream->port(),
    [self = shared_from_this()](boost::system::error_code ec,
                                const boost::asio::ip::tcp::resolver::results_type& res)
    {
      self->onResolve(ec,res);
    });
}

}
}
}