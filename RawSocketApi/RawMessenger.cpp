//
// Created by Arthur Motelevicz on 06/03/23.
//

#include "RawMessenger.h"
#include "RawSender.h"
#include "RawStream.h"

namespace bb
{

RawMessenger::RawMessenger(){
}

void  RawMessenger::sendMessage(const std::shared_ptr<network::rs::Stream>& stream,
                             const std::string& message,
                             network::rs::SendMessageCB cb){

  bool startSend = false;
  std::lock_guard<std::mutex> lock(_sendMutex);
  _messages.emplace(stream, message, cb);
  if (!_sendingMessage) {
    _sendingMessage = true;
    startSend = true;
  }

  if (startSend)
  {
    startSending();
  }
}

void RawMessenger::startSending()
{
  if(_messages.empty())
  {
    _sendingMessage = false;
    return;
  }

  _sendingMessage = true;
  auto& im = _messages.front();

  std::make_shared<network::rs::Sender>(im.stream)->send(im.message,
  [this, im](bool success)
  {
    if(im.callback)
      im.callback(success);

    _messages.pop();
    startSending();
  });
}
}