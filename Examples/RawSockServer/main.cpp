//
// Created by Arthur Motelevicz on 26/07/24.
//
#include <iostream>
#include <beastboys>

int main()
{
  bb::network::rs::server::RawServer s;
  std::thread worker([&](){
    s.start();
  });
  worker.detach();

  bool quit = false;
  while(!quit){
    std::string input;
    std::getline(std::cin, input);

    if(input == "stop"){
      std::cout << "Close all streams!!\n";
      s.stop();
    }
    if(input == "close"){
      std::cout << "Close all streams!!\n";
      s.disconnectAll();
    }
    else if(input == "quit"){
      quit = true;
    }
  }

  return EXIT_SUCCESS;
}