//
// Created by Arthur Motelevicz on 29/07/24.
//

#ifndef BEAST_BOYS_RAWDOORMAN_H
#define BEAST_BOYS_RAWDOORMAN_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "Doorman.h"

namespace bb::network::server::rs
{
  class RawDoorman : public bb::network::server::Doorman
  {
  public:
    using bb::network::server::Doorman::Doorman;

  protected:
    void onAccept(boost::system::error_code ec) override;
  };
}

#endif //BEAST_BOYS_RAWDOORMAN_H
