//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <boost/asio.hpp>

namespace bb::network::server
{
  class ServerState;
  class Connection;

  class Client : public std::enable_shared_from_this<Client> {
  public:
    inline static unsigned long idCounter = 0;

    Client(Connection *connection);

    unsigned long getId() const;

    Connection *getConnection();

  private:
    Connection *_connection{nullptr};
    unsigned long _id = 0;
  };
}


#endif //SOCKET_CLIENT_H


