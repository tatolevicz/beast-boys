//
// Created by Arthur Motelevicz on 26/07/24.
//
#include <iostream>
#include <beastboys>

int main()
{
  std::string message;

  bb::network::rs::server::Server s;

  s.setOnSendMessageCB([&](const std::string& msg){
    message = msg;
  });

  std::thread worker([&](){
    s.start(1234);
  });

  worker.detach();

  bool quit = false;
  while(!quit)
  {
    if(message.empty())
      continue;

    if(message == "stop\n"){
      std::cout << "Stop all streams!!\n";
      s.stop();
    }
    if(message == "close\n"){
      std::cout << "Close all streams!!\n";
      s.disconnectAll();
    }
    else if(message == "quit\n"){
      quit = true;
    }

    message = "";
  }

  return EXIT_SUCCESS;
}