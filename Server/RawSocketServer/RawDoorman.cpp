//
// Created by Arthur Motelevicz on 29/07/24.
//

#include "RawDoorman.h"
#include "ServerState.h"
#include "Connection.h"
#include <iostream>
namespace bb::network::server::rs
{
  void RawDoorman::onAccept(boost::system::error_code ec)
  {
    if (!ec)
    {
      std::make_shared<bb::network::server::Connection>(std::move(_sock), _serverState)->run();
    } else {
      std::cerr << "Error: " << ec.value() << " Msg: " << ec.message() << "\n";
    }
    run();
  }
}