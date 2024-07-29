//
// Created by Arthur Motelevicz on 06/03/23.
//

#include "Messenger.h"
#include "Sender.h"
#include "Stream.h"

namespace bb{
    Messenger::Messenger(){
    }

    void  Messenger::sendMessage(const std::shared_ptr<network::ws::Stream>& stream,
                                 const std::string& message,
                                 network::ws::SendMessageCB cb){

        bool startSend = false;
        std::lock_guard<std::mutex> lock(_sendMutex);
        _messages.emplace(stream, message, cb);
        if (!_sendingMessage) {
            _sendingMessage = true;
            startSend = true;
        }

        if (startSend) {
            startSending();
        }
    }

    void Messenger::startSending(){
        if(_messages.empty()){
            _sendingMessage = false;
            return;
        }

        _sendingMessage = true;
        auto& im = _messages.front();

        std::make_shared<network::ws::Sender>(im.stream)->send(im.message,[this, im](bool success){
            if(im.callback)
                im.callback(success);

            _messages.pop();
            startSending();
        });
    }
}