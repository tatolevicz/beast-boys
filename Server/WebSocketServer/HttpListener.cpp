//
// Created by Arthur Motelevicz on 26/02/23.
//

#include "HttpListener.h"
#include "ServerState.h"
#include "HandShaker.h"
#include "Logger.h"

//server beast
namespace bb::network::server::ws
{
HttpListener::HttpListener(boost::asio::ip::tcp::socket sock,
                           std::shared_ptr<ServerState> serverState) :
_sock(std::move(sock)),
_serverState(std::move(serverState))
{

}

void HttpListener::onRead(boost::system::error_code ec, std::size_t bytes){

    RETURN_IF_ASIO_ERROR_(ec)

    if(boost::beast::websocket::is_upgrade(_req))
    {
        std::make_shared<HandShaker>(std::move(_sock),_serverState)->run(_req);
        return;
    }
}

void HttpListener::run() {
    boost::beast::http::async_read(
      _sock,
      _buffer,
      _req,
      [self = shared_from_this()](boost::system::error_code ec,std::size_t bytes){
          self->onRead(ec, bytes);
    });
}

}
