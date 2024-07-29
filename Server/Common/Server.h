//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <boost/asio.hpp>
#include "Server/Common/ServerState.h"
#include "Server/Common/Doorman.h"

namespace bb::network::server
{
  class ServerState;
  class Doorman;

  using OnSendMessageCallback = std::function<void(const std::string& message)>;

  class Server : public std::enable_shared_from_this<Server>
  {
  public:
    Server() = default;

    template<class DOORMAN_TYPE>
    void start(uint32_t port)
    {
      static_assert(std::is_base_of<bb::network::server::Doorman, DOORMAN_TYPE>::value, "DOORMAN_TYPE must be derived from Doorman");

      _endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port);
      _serverState = std::make_shared<ServerState>();

      if(_onSendMessageCb)
        _serverState->setOnSendMessageCB(_onSendMessageCb);

      _doorMan = std::make_shared<DOORMAN_TYPE>(_ioc, _endpoint, _serverState);
      _doorMan->run();
      _ioc.run();
    }

    void stop();
    void disconnectAll();
    void broadcast(const std::string& message) const;
    void setOnSendMessageCB(const OnSendMessageCallback& cb);
  private:
    OnSendMessageCallback _onSendMessageCb{nullptr};
    std::shared_ptr<ServerState> _serverState{nullptr};
    std::shared_ptr<Doorman> _doorMan{nullptr};
    boost::asio::io_context _ioc;
    boost::asio::ip::tcp::endpoint _endpoint;
  };
}



#endif //SOCKET_SERVER_H
