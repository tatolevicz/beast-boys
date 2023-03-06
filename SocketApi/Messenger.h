//
// Created by Arthur Motelevicz on 06/03/23.
//

#ifndef BEAST_BOYS_MESSENGER_H
#define BEAST_BOYS_MESSENGER_H

#include "WebsocketTypes.h"

namespace bb{

    namespace network::ws {
        class Stream;
    }
    class Messenger{
    public:
        Messenger();
        /**
         * Send a message to the stream
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
        void sendMassage(const std::shared_ptr<network::ws::Stream>& stream,
                         const std::string& message,
                         network::ws::SendMessageCB cb = nullptr);

    private:
    };

}
#endif //BEAST_BOYS_MESSENGER_H
