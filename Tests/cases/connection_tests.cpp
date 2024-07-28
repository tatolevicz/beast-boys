//
// Created by Arthur Motelevicz on 28/07/24.
//
#include <beastboys>
#include <thread>
#include <string>
#include "RawServer.h"
#include "catch2/catch.hpp"
#include "TestHelpers.h"

TEST_CASE("Socket Connection Tests", "[socket]")
{
  bb::ErrorManager::getInstance().registerCallback(errorCallback);

  bb::network::rs::server::RawServer server;
  std::shared_ptr<bb::RawStreamer> streamer(new bb::RawStreamer());

  std::string message;

  std::thread serverThread([&]()
                           {
                             server.start(1234);
                           });

  // Give the server some time to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  SECTION("Test invalid Server")
  {
    auto stream = streamer->openStream("zxcvzxcvz ","1234","", nullptr);
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(!streamPtr->isOpen());
  }

  SECTION("Test Basic Connection Success")
  {
    auto stream = streamer->openStream("127.0.0.1","1234","", nullptr);
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(streamPtr->isOpen());
  }

  SECTION("Test Connection Resolving Success")
  {
    auto stream = streamer->openStream("localhost","1234","", nullptr);
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(streamPtr->isOpen());
  }

  SECTION("Test Forced Disconnection by Server")
  {
    bool finalResponse = false;
    auto stream = streamer->openStream("localhost", "1234", "",
                                       [&](bool success, const std::string& data, auto stream)
                                       {
                                         if (!success)
                                         {
                                           REQUIRE(!success);
                                           REQUIRE(data == "End of file");
                                           finalResponse= true;
                                         }
                                       });

    auto streamPtr = stream.lock();


    if (!streamPtr)
    {
      std::cerr << "Failed to lock stream." << std::endl;
      REQUIRE(false);
    }

//    // sleep with small chucks to improve waiting time
//    auto wait_for_open = [&, streamPtr](auto lamb)
//    {
//      for (int i = 0; i < 200; ++i) {
//        if (lamb())
//          break;
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//      }
//    };

    waitForCondition([streamPtr]() -> bool
    {
      return streamPtr->isOpen();
    });

    auto messenger = std::make_unique<bb::RawMessenger>();
    std::string testMessage = "close";

    messenger->sendMessage(streamPtr, testMessage, [](bool success)
    {
      REQUIRE(success);
    });

    server.setOnSendMessageCB([&](const std::string& message)
    {
      handleServerMessages(server, message);
    });

    // Give time to wait response from server
    waitForCondition([&]() -> bool
    {
      return finalResponse;
    });
  }

  server.stop();
  serverThread.join();
}