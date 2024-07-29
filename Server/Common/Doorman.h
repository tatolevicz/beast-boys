//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_DOORMAN_H
#define SOCKET_DOORMAN_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace bb::network::server
{
  class ServerState;

  class Doorman : public std::enable_shared_from_this<Doorman>
  {
  public:
    Doorman(boost::asio::io_context &ioc,
            boost::asio::ip::tcp::endpoint &endpoint,
            std::shared_ptr<ServerState> serverState);
    void run();
    void stop();

  protected:
    virtual void onAccept(boost::system::error_code ec) = 0;

    std::shared_ptr<ServerState> _serverState{nullptr};
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::socket _sock;
    boost::asio::ip::tcp::endpoint _endpoint;
  };

}


#endif //SOCKET_DOORMAN_H
