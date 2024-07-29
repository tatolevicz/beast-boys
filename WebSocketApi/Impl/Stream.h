//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BINANCEBEAST_STREAM_H
#define BINANCEBEAST_STREAM_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include "WebsocketTypes.h"

namespace bb::network::ws
{
  class Stream : public std::enable_shared_from_this<Stream>
  {
  public:

      friend class Connector;

      Stream(boost::asio::io_context &ioc,
             std::string host,
             std::string port,
             std::string target,
             boost::asio::ssl::context &sslContext,
             StreamCB cb
      );

      Stream(boost::asio::io_context &ioc,
             std::string host,
             std::string port,
             std::string target,
             StreamCB cb
      );

      ~Stream();

      uint32_t getId() const;
      std::string host() const;
      std::string port() const;
      std::string target() const;
      bool usesSSL() const;

      boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>& getSocketSSL();
      boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& getSocket();
      void feedData(const std::string& data);
      void connectionAborted(boost::system::error_code ec);
      void stop();
      void stopWithCloseCallbackTriggered();

      //should be at max 125 bytes
      void ping(const std::string& payload = "no payload");

      void setCloseStreamCallback(const CloseStreamCallback& cb);
      void setPongStreamCallback(const PongStreamCallback& cb);
      void setPingStreamCallback(const PingStreamCallback& cb);
      bool wasClosedByServer();
      bool wasClosedByClient();
      bool isOpen() const;

  private:
      void internalStop();
      //should be called by the connector (that is a friend class) when the socket is connected
      void setWatchControlMessages();

      inline static uint32_t id = 0;
      std::shared_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>> _socket{nullptr};
      std::shared_ptr<boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>> _socketSSL{nullptr};

      std::string _host;
      std::string _port;
      std::string _target;
      uint32_t _id;
      bool _usesSSL{true};

      //flag to handle when the stream was closed from control message close properly
      bool _wasClosedByServer = false;
      bool _wasClosedByClient = false;

      CloseStreamCallback _closeStreamCB{nullptr};
      PongStreamCallback _pongStreamCB{nullptr};
      PingStreamCallback _pingStreamCB{nullptr};
      StreamCB _cb{nullptr};
  };

}

#endif //BINANCEBEAST_STREAM_H
