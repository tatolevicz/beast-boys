//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef WEBSOCKET_DOORMAN_H
#define WEBSOCKET_DOORMAN_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "Doorman.h"

namespace bb::network::server::ws
{
  class WebSocketDoorman : public bb::network::server::Doorman
  {
  public:
    using bb::network::server::Doorman::Doorman;

  protected:
    void onAccept(boost::system::error_code ec) override;
  };
}

#endif //WEBSOCKET_DOORMAN_H
