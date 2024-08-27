//
// Created by Arthur Motelevicz on 26/07/24.
//
#include <iostream>
#include <beastboys>

int main()
{
  std::string message;

  bb::network::server::Server s;

  s.setOnSendMessageCB([&](const std::string& msg){
    message = msg;
  });

  std::thread worker([&](){
    s.start<bb::network::server::rs::RawDoorman>(1234);
  });

  bool quit = false;
  while(!quit)
  {
    if(message.empty())
      continue;

    if(message == "stop\n"){
      lg(mgutils::Info) << "Stop all streams!!\n";
      s.stop();
    }
    if(message == "close\n"){
      lg(mgutils::Info) << "Close all streams!!\n";
      s.disconnectAll();
    }
    else if(message == "quit\n"){
      quit = true;
      s.stop();
    }

    message = "";
  }

  if(worker.joinable())
    worker.join();

  return EXIT_SUCCESS;
}