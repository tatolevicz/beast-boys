//
// Created by Arthur Motelevicz on 28/07/24.
//
#include <beastboys>
#include <thread>
#include <string>
#include "catch2/catch.hpp"
#include "TestHelpers.h"

TEST_CASE("Socket Connection Tests", "[socket]")
{
// Subscribe to error events
  auto &errorManager = mgutils::ErrorManager::instance();
  boost::signals2::connection errorConnection;

  bb::network::server::Server server;
  std::shared_ptr<bb::RawStreamer> streamer(new bb::RawStreamer());

  std::string message;

  std::thread serverThread([&]()
  {
   server.start<bb::network::server::rs::RawDoorman>(1234);
  });

  // Give the server some time to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  SECTION("Test invalid Server")
  {
    errorConnection = errorManager.subscribe([&](const mgutils::ErrorInfo& error)
    {
      infoCallback(error);
      // expected error here is 1 and 89 (cient and server respectively)
      REQUIRE(error.errorCode == 1);
    });

    auto stream = streamer->openStream("zxcvzxcvz ","1234","", [](bool success, const std::string& data, RawSharedStream stream){});
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(!streamPtr->isOpen());
  }

  SECTION("Test Basic Connection Success")
  {
    // no error expected
    errorConnection = errorManager.subscribe(&errorCallback);

    auto stream = streamer->openStream("127.0.0.1","1234","", [](bool success, const std::string& data, RawSharedStream stream){});
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(streamPtr->isOpen());
  }

  SECTION("Test Connection Resolving Success")
  {
    errorConnection = errorManager.subscribe(&errorCallback);
    auto stream = streamer->openStream("localhost","1234","", [](bool success, const std::string& data, RawSharedStream stream){});
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(streamPtr->isOpen());
  }

  SECTION("Test Forced Disconnection by Server")
  {
    errorConnection = errorManager.subscribe([&](const mgutils::ErrorInfo& error)
    {
      infoCallback(error);
      // expected error here is 2 and 89 (cient and server respectively)
      auto condition = error.errorCode == 2 || error.errorCode == 89;
      REQUIRE(condition);
    });

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
      lg(mgutils::Error) << "Failed to lock stream.";
      REQUIRE(false);
    }

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

  errorConnection.disconnect();
  server.stop();
  serverThread.join();
}