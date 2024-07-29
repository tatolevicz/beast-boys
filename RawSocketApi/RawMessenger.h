//
// Created by Arthur Motelevicz on 06/03/23.
//

#ifndef RAW_MESSENGER_H
#define RAW_MESSENGER_H

#include "RawSocketTypes.h"
#include <queue>

namespace bb
{

namespace network::rs
{
    class Stream;
}

class RawMessenger
{
public:
  struct InternalMessage {
    std::shared_ptr<network::rs::Stream> stream;
    std::string message;
    network::rs::SendMessageCB  callback;

    InternalMessage(const std::shared_ptr<network::rs::Stream> s, std::string m, network::rs::SendMessageCB cb)
            : stream(std::move(s)), message(std::move(m)), callback(std::move(cb)) {}
  };

  RawMessenger();

  void sendMessage(const std::shared_ptr<network::rs::Stream>& stream,
                   const std::string& message,
                   network::rs::SendMessageCB cb = nullptr);

private:
  std::mutex _sendMutex;
  void startSending();
  std::queue<InternalMessage> _messages;
  bool _sendingMessage{false};
};

}
#endif //RAW_MESSENGER_H
