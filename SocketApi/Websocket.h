//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef BINANCEBEAST_WEBSOCKET_H
#define BINANCEBEAST_WEBSOCKET_H

#include "WebsocketTypes.h"

namespace bb{
namespace network{
namespace ws{

    class WebsocketImpl;
    class Stream;
    class Websocket{
    public:
        Websocket();
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
        std::weak_ptr<Stream> openStream(std::string baseUrl,
                                         std::string port,
                                         std::string endPoint,
                                         bool usesSSL,
                                         StreamCB cb);


    private:
        std::shared_ptr<WebsocketImpl> _impl{nullptr};
    };

}
}
}
#endif //BINANCEBEAST_WEBSOCKET_H
