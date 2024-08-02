//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "RawStreamer.h"
#include "RawSocketImpl.h"
#include "RawStream.h"

namespace bb{

RawStreamer::RawStreamer(){
  _impl = std::make_shared<network::rs::RawSocketImpl>();
}

std::weak_ptr<network::rs::Stream>  RawStreamer::openStream(std::string baseUrl,
                                                         std::string port,
                                                         std::string endPoint,
                                                         network::rs::StreamCB cb,
                                                         char delimiter)
{
    return _impl->openStream(
            std::move(baseUrl),
            std::move(port),
            std::move(endPoint),
            std::move(cb),
            delimiter);
}

std::weak_ptr<network::rs::Stream>  RawStreamer::openStream(std::string baseUrl,
                                                            std::string port,
                                                            std::string endPoint,
                                                            network::rs::StreamCB2 cb,
                                                            char delimiter)
{
  return _impl->openStream(
      std::move(baseUrl),
      std::move(port),
      std::move(endPoint),
      std::move(cb),
      delimiter);
}

}