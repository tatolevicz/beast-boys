//
// Created by Arthur Motelevicz on 28/07/24.
//
//
// Created by Arthur Motelevicz on 28/07/24.
//
#include <beastboys>
#include <thread>
#include <string>
#include "RawServer.h"
#include "catch2/catch.hpp"
#include "TestHelpers.h"

TEST_CASE("Socket Messaging Tests", "[socket]")
{
  // Subscribe to error events
  auto &errorManager = bb::ErrorManager::instance();
  boost::signals2::connection errorConnection;

  bb::network::rs::server::RawServer server;
  std::shared_ptr<bb::RawStreamer> streamer(new bb::RawStreamer());

  std::string message;

  std::thread serverThread([&]()
  {
   server.start(1234);
  });

  // Give the server some time to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  SECTION("Test Sending and Receiving Messages")
  {
    errorConnection = errorManager.subscribe(&errorCallback);

    try
    {
      auto stream = streamer->openStream("localhost", "1234", "",
      [](bool success, const std::string& data, const auto &stream)
      {
        if (!success)
        {
           std::cout << "Stream closed with msg: " << data << "\n\n";
           return;
        }

        // Work with your streamed data here
        LOG_INFO(data);
      });

      std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened

      auto messenger = std::make_unique<bb::RawMessenger>();

      std::string testMessage = "Hello, Server!";
      auto streamPtr = stream.lock();
      if (streamPtr)
      {
        // Use a promise and future to wait for the result in the main thread
        std::promise<bool> sendPromise;
        std::future<bool> sendFuture = sendPromise.get_future();

        std::promise<std::string> sendMsgPromise;
        std::future<std::string> sendMsgFuture = sendMsgPromise.get_future();

        // Server's callback is called when it send messages to its clients.
        // The test server send everything it receives to all clients and after
        // that it call this callback.
        server.setOnSendMessageCB([&](const std::string& msg)
        {
          sendMsgPromise.set_value(msg);
        });

        //Client messenger uses the stream to send messages and when it
        // it queues all the messages and when it finish sending this message (or some error occur) it calls the callback
        messenger->sendMessage(streamPtr, testMessage,
        [&sendPromise](bool success)
        {
          sendPromise.set_value(success);
        });

        // Wait for the result in the main thread with a timeout
        if (sendFuture.wait_for(std::chrono::seconds(5)) == std::future_status::ready)
        {
          bool success = sendFuture.get();
          REQUIRE(success);
        }
        else
        {
          std::cerr << "Timeout waiting for sendMessage callback." << std::endl;
          REQUIRE(false); // Force test failure on timeout
        }

        // Wait for the result in the main thread with a timeout
        if (sendMsgFuture.wait_for(std::chrono::seconds(5)) == std::future_status::ready)
        {
          std::string receivedMsg = sendMsgFuture.get();
          REQUIRE(receivedMsg == testMessage);
        } else
        {
          std::cerr << "Timeout waiting for getting message received on server's callback." << std::endl;
          REQUIRE(false); // Force test failure on timeout
        }
      }
      else
      {
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

  errorConnection.disconnect();
  server.stop();
  serverThread.join();
}