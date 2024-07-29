//
// Created by Arthur Motelevicz on 29/07/24.
//

#ifndef BEAST_BOYS_CONNECTION_H
#define BEAST_BOYS_CONNECTION_H

#include <boost/asio.hpp>

namespace bb::network::server
{
  class ServerState;

  class Connection
  {
  public:
    Connection(std::shared_ptr<ServerState> serverState);
    virtual ~Connection();
    virtual void run() = 0;
    virtual void send(const std::string& message) = 0;
    virtual void disconnect() = 0;
  protected:
    std::shared_ptr<ServerState> _serverState{nullptr};
  };
}

#endif //BEAST_BOYS_CONNECTION_H
