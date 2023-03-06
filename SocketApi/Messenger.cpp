//
// Created by Arthur Motelevicz on 06/03/23.
//

#include "Messenger.h"
#include "Sender.h"
#include "Stream.h"

namespace bb{
    Messenger::Messenger(){
    }

    void  Messenger::sendMassage(const std::shared_ptr<network::ws::Stream>& stream,
                                                               const std::string& message,
                                                               network::ws::SendMessageCB cb){

        std::make_shared<network::ws::Sender>(stream)->send(message,std::move(cb));
    }

}