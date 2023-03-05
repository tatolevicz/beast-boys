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
    class Websocket{
    public:
        Websocket();
        /**
         * Create a stream and call the cb when with json data provided
         *
         * @param baseUrl   :  expected a base url for: wss://<base_url>:<port>/<endpoint>
         * @param port      :  expected a port for: wss://<base_url>:<port>/<endpoint> -> if an empty port is passed so 443 will be used
         * @param endPoint  :  expected an endPoint for: wss://<base_url>:<port><endpoint> -> *Note the lack of slash between the port and endpoint so provide it with the endpoint ex: "/something/great"
         * @param cb        :  callback with json data as parameter.
         * @return          :  returns an int id of the stream that should be used to close it.
         */
        int openStream(std::string baseUrl,
                       std::string port,
                       std::string endPoint,
                       bool usesSSL);


        /**
         * Close the stream with its id.
         * @param id
         */
        void closeStream(const int id);

        bool hasWork();


    private:
        std::shared_ptr<WebsocketImpl> _impl{nullptr};
    };

}
}
}
#endif //BINANCEBEAST_WEBSOCKET_H
