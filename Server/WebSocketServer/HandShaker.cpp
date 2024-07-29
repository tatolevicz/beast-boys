//
// Created by Arthur Motelevicz on 29/07/24.
//

#include "HandShaker.h"
#include "ServerState.h"
#include "Logger.h"
#include "WSConnection.h"

namespace bb::network::server::ws
{
  HandShaker::HandShaker(boost::asio::ip::tcp::socket sock, std::shared_ptr<ServerState> serverState):
  _serverState(std::move(serverState)),
  _sockStream(std::move(sock))
  {}

  void HandShaker::onHandShake(boost::system::error_code ec)
  {
    RETURN_IF_ASIO_ERROR_(ec)
    std::make_shared<WSConnection>(std::move(_sockStream), _serverState)->run();
  }

  void HandShaker::run(boost::beast::http::request<boost::beast::http::string_body>& _req)
  {
    _sockStream.async_accept(_req,
    [self = shared_from_this()](boost::system::error_code ec)
    {
      self->onHandShake(ec);
    });
  }
}