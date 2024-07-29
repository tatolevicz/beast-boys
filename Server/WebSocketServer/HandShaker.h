//
// Created by Arthur Motelevicz on 29/07/24.
//

#ifndef BEAST_BOYS_HANSHAKER_H
#define BEAST_BOYS_HANSHAKER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace bb::network::server
{
  class ServerState;
}

namespace bb::network::server::ws
{
  class HandShaker : public std::enable_shared_from_this<HandShaker>
  {
  public:
    HandShaker(boost::asio::ip::tcp::socket sock, std::shared_ptr<bb::network::server::ServerState> serverState);
    void run(boost::beast::http::request<boost::beast::http::string_body>& _req);

  private:
    void onHandShake(boost::system::error_code ec);
    std::shared_ptr<bb::network::server::ServerState> _serverState{nullptr};
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> _sockStream;
    boost::beast::flat_buffer _buffer;
    std::vector<std::string> _messageQueue;
  };

}

#endif //BEAST_BOYS_HANSHAKER_H
