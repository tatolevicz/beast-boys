//
// Created by Arthur Motelevicz on 26/07/24.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"
#include "beastboys"
#include <thread>
#include <string>
#include "RawServer.h"

TEST_CASE("Socket Connection Test", "[socket]") {
  bb::network::rs::server::RawServer server;
  std::shared_ptr<bb::RawStreamer> streamer(new bb::RawStreamer());

  std::string receivedMessage;

  server.setOnSendMessageCB([&](const std::string& msg) {
    receivedMessage = msg;
  });

  std::thread serverThread([&]() {
    server.start(1234);
  });

  // Give the server some time to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  SECTION("Test Basic Connection Success")
  {
    REQUIRE(streamer->openStream("127.0.0.1","1234","", nullptr).use_count() > 0);
  }

  SECTION("Test Connection Resolving Success")
  {
    REQUIRE(streamer->openStream("localhost","1234","", nullptr).use_count() > 0);
  }

  SECTION("Test Sending and Receiving Messages") {
    try
    {
      auto stream = streamer->openStream("localhost", "1234", "",
      [](bool success, const std::string& data, auto stream) {
        if (!success)
        {
         std::cout << "Stream closed with msg: " << data << "\n\n";
         return;
        }

        // Work with your streamed data here
        std::cout << data << "\n\n";
      });

      std::this_thread::sleep_for(std::chrono::seconds(2)); // time to stream be opened

      auto messenger = std::make_unique<bb::RawMessenger>();

      std::string testMessage = "Hello, Server!";
      auto streamPtr = stream.lock();
      if (streamPtr)
      {
        // Use a promise and future to wait for the result in the main thread
        std::promise<bool> sendPromise;
        std::future<bool> sendFuture = sendPromise.get_future();

        messenger->sendMessage(streamPtr, testMessage,
        [&sendPromise](bool success)
        {
         sendPromise.set_value(success);
        });

        // Wait for the result in the main thread
        bool success = sendFuture.get();
        REQUIRE(success);
      } else {
        std::cerr << "Failed to lock stream." << std::endl;
        REQUIRE(false); // Force test failure if stream is not valid
      }
    } catch (const std::exception& e)
    {
      std::cerr << "Exception caught: " << e.what() << std::endl;
      REQUIRE(false); // Force test failure on exception
    } catch (...)
    {
      std::cerr << "Unknown exception caught" << std::endl;
      REQUIRE(false); // Force test failure on unknown exception
    }
  }

//  SECTION("Test Sending and Receiving Messages")
//  {
//    auto stream = streamer->openStream("localhost","1234","",
//    [](bool success, const std::string& data, auto stream)
//    {
//     if(!success) {
//       std::cout << "Stream closed with msg: " << data << "\n\n";
//       return;
//     }
//
//     //Work with your streamed data here
//     std::cout << data << "\n\n";
//    });
//
//    std::this_thread::sleep_for(std::chrono::seconds(2)); // time to stream be opened
//
//    auto messenger  = std::make_unique<bb::RawMessenger>();
//
//    std::string testMessage = "Hello, Server!";
//    messenger->sendMessage(stream.lock(),
//    testMessage,
//    [](bool success)
//    {
//      REQUIRE(success);
//    });

//    while(stream.lock())
//    {
//      std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for the message to be processed
//      messenger->sendMessage(stream.lock(),
//      testMessage,
//      [](bool success) {
//       if (success)
//         std::cout << "Msg enviada com sucesso!\n";
//       else
//         std::cerr << "Msg nao enviada!\n";
//      });
//    }
//    REQUIRE(receivedMessage == testMessage);
//  }

//
//  SECTION("Test Connection Failure") {
//    // Attempt to connect to an invalid IP or port
//    REQUIRE(client.connect("256.256.256.256", 8080) == false);
//    REQUIRE(client.isConnected() == false);
//  }

  server.stop();
  serverThread.join();
}