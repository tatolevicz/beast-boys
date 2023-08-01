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

        void sendMessage(const std::shared_ptr<network::ws::Stream>& stream,
                         const std::string& message,
                         network::ws::SendMessageCB cb = nullptr);

        //TODO:: fazer metodos para receber ping e enviar pong
        //exemplo do control callback da pong message na BINAPI
//        m_ws.control_callback(
//        [this]
//        (boost::beast::websocket::frame_type kind, boost::beast::string_view payload) mutable {
//            (void)kind; (void) payload;
//            //std::cout << "control_callback(" << this << "): kind=" << static_cast<int>(kind) << ", payload=" << payload.data() << std::endl;
//            m_ws.async_pong(
//                    boost::beast::websocket::ping_data{}
//                    ,[](boost::beast::error_code ec)
//                    { (void)ec; /*std::cout << "control_callback_cb(" << this << "): ec=" << ec << std::endl;*/ }
//            );
//        }
//        );

    private:
    };

}
#endif //BEAST_BOYS_MESSENGER_H
