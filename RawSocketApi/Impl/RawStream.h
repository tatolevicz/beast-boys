//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef RAW_STREAM_H
#define RAW_STREAM_H

#include <boost/asio.hpp>
#include "RawSocketTypes.h"

namespace bb::network::rs
{
  class Stream : public std::enable_shared_from_this<Stream>
  {
  public:
    Stream(boost::asio::io_context &ioc,
           std::string host,
           std::string port,
           std::string target,
           StreamCB cb,
           char delimiter = '\0'
    );

    Stream(boost::asio::io_context &ioc,
           std::string host,
           std::string port,
           std::string target,
           StreamCB2 cb,
           char delimiter = '\0'
    );


    ~Stream();

    uint32_t getId() const;
    std::string host() const;
    std::string port() const;
    std::string target() const;
    boost::asio::ip::tcp::socket& getSocket();
    void feedData(const std::string& data);
    void feedData(const char* data, size_t length);
    void connectionAborted(boost::system::error_code ec);
    void stop();
    void stopWithCloseCallbackTriggered();

    //should be at max 125 bytes
//    void ping(const std::string& payload = "no payload");

    void setCloseStreamCallback(const CloseStreamCallback& cb);
    void setPongStreamCallback(const PongStreamCallback& cb);
    void setPingStreamCallback(const PingStreamCallback& cb);
    bool wasClosedByServer() const;
    bool wasClosedByClient() const;
    bool isOpen() const;
    char getReadUntilDelimiter() const;

  private:
    void internalStop();
    inline static uint32_t id = 0;
    std::shared_ptr<boost::asio::ip::tcp::socket> _socket{nullptr};

    std::string _host;
    std::string _port;
    std::string _target;
    uint32_t _id;
    char _readUntilDelimiter = '\0';


    //flag to handle when the stream was closed from control message close properly
    bool _wasClosedByServer = false;
    bool _wasClosedByClient = false;

    CloseStreamCallback _closeStreamCB{nullptr};
    PongStreamCallback _pongStreamCB{nullptr};
    PingStreamCallback _pingStreamCB{nullptr};
    StreamCB _cb{nullptr};
    StreamCB2 _cb2{nullptr};
  };
}

#endif //RAW_STREAM_H
