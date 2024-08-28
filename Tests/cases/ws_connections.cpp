//
// Created by Arthur Motelevicz on 29/07/24.
//
#include <beastboys>
#include <thread>
#include <string>
#include "catch2/catch.hpp"
#include "TestHelpers.h"

TEST_CASE("WebSocket Connection Tests", "[websocket]")
{
// Subscribe to error events
  auto &errorManager = mgutils::ErrorManager::instance();
  boost::signals2::connection errorConnection;

  bb::network::server::Server server;
  std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

  std::string message;

  std::thread serverThread([&]()
  {
    server.start<bb::network::server::ws::WebSocketDoorman>(1234);
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

    auto stream = streamer->openStream("zxcvzxcvz ","1234","", false, nullptr);
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(!streamPtr->isOpen());
  }

//  SECTION("Test Client On Postman")
//  {
//    errorConnection = errorManager.subscribe(&errorCallback);
//    std::this_thread::sleep_for(std::chrono::seconds(20)); // time to stream be opened
//  }
  SECTION("Test Basic Client Connection")
  {
    // no error expected
    errorConnection = errorManager.subscribe(&errorCallback);

    auto stream = streamer->openStream("127.0.0.1","1234","", false, nullptr);
    auto streamPtr = stream.lock();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // time to stream be opened
    REQUIRE(streamPtr->isOpen());
  }

  SECTION("Test Connection Resolving Success")
  {
    errorConnection = errorManager.subscribe(&errorCallback);
    auto stream = streamer->openStream("localhost","1234","", false, nullptr);
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
    auto stream = streamer->openStream("localhost", "1234", "", false,
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

    auto messenger = std::make_unique<bb::Messenger>();

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

TEST_CASE("WebSocket SSL Tests", "[websocket][ssl]")
{

  auto &errorManager = mgutils::ErrorManager::instance();
  boost::signals2::connection errorConnection;

  std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

  std::promise<bool> sendPromise;
  std::future<bool> sendFuture = sendPromise.get_future();

  std::promise<std::string> receivePromise;
  std::future<std::string> receiveFuture = receivePromise.get_future();

  SECTION("Test SSL Connection with Public WebSocket Server")
  {
    errorConnection = errorManager.subscribe(&errorCallback);

    bool skippedFirst = false;
    auto stream = streamer->openStream("ws.ifelse.io", "443", "", true,  // 'true' for SSL
     [&](bool success, const std::string &data, const auto &stream) {
       if (!success) {
         LOG_ERROR("Stream closed with msg: " + data);
         return;
       }

       if(skippedFirst)
        receivePromise.set_value(data);

       skippedFirst = true;
       LOG_INFO(data);
     });

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Tempo para o stream ser aberto

    auto streamPtr = stream.lock();
    if (streamPtr)
    {
      REQUIRE(streamPtr->usesSSL());

      std::string testMessage = "Hello, Secure WebSocket!";

      auto messenger = std::make_unique<bb::Messenger>();

      //Client messenger uses the stream to send messages and when it
      // it queues all the messages and when it finish sending this message (or some error occur) it calls the callback
      messenger->sendMessage(streamPtr, testMessage,
        [&sendPromise](bool success)
        {
         sendPromise.set_value(success);
        });


      if (sendFuture.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
        bool success = sendFuture.get();
        REQUIRE(success);
      } else {
        LOG_ERROR("Timeout waiting for sendMessage callback.");
        REQUIRE(false); // Forçar falha no teste em caso de timeout
      }

      if (receiveFuture.wait_for(std::chrono::seconds(5)) == std::future_status::ready)
      {
        std::string receivedMsg = receiveFuture.get();
        REQUIRE(receivedMsg == testMessage);
      } else {
        LOG_ERROR("Timeout waiting for message from WebSocket server.");
        REQUIRE(false); // Forçar falha no teste em caso de timeout
      }
    } else {
      LOG_ERROR("Failed to lock stream.");
      REQUIRE(false); // Forçar falha no teste se o stream não for válido
    }

  }

  // Outros testes de SSL podem ser adicionados aqui

  errorConnection.disconnect();
}
