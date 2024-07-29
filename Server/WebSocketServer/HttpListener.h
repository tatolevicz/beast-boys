//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_HTTPLISTENER_H
#define SOCKET_HTTPLISTENER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace bb::network::server
{
  class ServerState;
}

//server beast
namespace bb::network::server::ws
{
  class HttpListener : public std::enable_shared_from_this<HttpListener> {
  public:
      HttpListener(boost::asio::ip::tcp::socket sock,
                   std::shared_ptr<bb::network::server::ServerState> sharedState);
      void run();
  private:
      void onRead(boost::system::error_code ec, std::size_t bytes);
      boost::beast::flat_buffer _buffer;
      boost::beast::http::request<boost::beast::http::string_body> _req;
      std::shared_ptr<bb::network::server::ServerState> _serverState{nullptr};
      boost::asio::ip::tcp::socket _sock;
  };

}


#endif //SOCKET_HTTPLISTENER_H
