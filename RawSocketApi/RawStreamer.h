//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef RAW_STREAMER_H
#define RAW_STREAMER_H

#include "RawSocketTypes.h"

namespace bb
{

namespace network::rs
{
    class RawSocketImpl;
    class Stream;
}

class RawStreamer
{
public:
  RawStreamer();
  /**
  * Create a stream and call the cb when with json data provided
  *
  * @param baseUrl   :  expected a base url for: wss://<base_url>:<port>/<endpoint>
  * @param port      :  expected a port for: wss://<base_url>:<port>/<endpoint> -> if an empty port is passed so 443 will be used
  * @param endPoint  :  expected an endPoint for: wss://<base_url>:<port><endpoint> ->
  *                     *Note the lack of slash between the port and endpoint so provide it with the endpoint ex: "/something/great"
  *                     if an empty target is passed so "/" will be used .
  *
  * @param usesSSL   :  if the socket url is "wss://" pass true, if it is just "ws://" pass false to it. Default value is true.
  * @param cb        :  callback called when data is arrives at the stream. The cb signature is (bool success, const std::string& data).
  *                      Check the <url address> for possible errors and messages.
  * @return          :  Returns a weak pointer to the stream that could be used in a loop while the stream is alive. More in the examples.
  */
  std::weak_ptr<network::rs::Stream> openStream(std::string baseUrl,
                                   std::string port,
                                   std::string endPoint,
                                   network::rs::StreamCB cb,
                                   char delimiter = '\0');

  std::weak_ptr<network::rs::Stream> openStream(std::string baseUrl,
                                                std::string port,
                                                std::string endPoint,
                                                network::rs::StreamCB2 cb,
                                                char delimiter = '\0');


private:
  std::shared_ptr<network::rs::RawSocketImpl> _impl{nullptr};
};

}

#endif //RAW_STREAMER_H
