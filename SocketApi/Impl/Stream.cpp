//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "Stream.h"
#include <iostream>

namespace bb {
namespace network {
namespace ws {

//Stream::Stream(boost::asio::io_context &ioc, std::string host, std::string port, std::string target, bool usesSSL):
//_ssl_context(boost::asio::ssl::context::sslv23_client),
//_host(std::move(host)),
//_port(std::move(port)),
//_target(std::move(target)),
//_usesSSL(usesSSL){
//    _id = ++id;
//
//    if(_usesSSL)
//    {
//        _ssl_context.set_default_verify_paths();
//        _ssl_context.set_verify_mode(boost::asio::ssl::verify_peer);
//
//        _socketSSL = std::make_shared<boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>>(ioc,_ssl_context);
//    }
//    else{
//        _socket = std::make_shared<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>(ioc);
//    }
//}

Stream::Stream(boost::asio::io_context &ioc,
               std::string host,
               std::string port,
               std::string target,
               boost::asio::ssl::context& ssl_context,
               StreamCB cb):
_host(std::move(host)),
_port(std::move(port)),
_target(std::move(target)),
_usesSSL(true),
_cb(cb)
{
    _id = ++id;
    _socketSSL = std::make_shared<boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>>(ioc,ssl_context);
}


Stream::Stream(boost::asio::io_context &ioc,
               std::string host,
               std::string port,
               std::string target,
               StreamCB cb):
_host(std::move(host)),
_port(std::move(port)),
_target(std::move(target)),
_usesSSL(false),
_cb(cb){
    _id = ++id;
    _socket = std::make_shared<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>(ioc);
}


uint32_t Stream::getId() const{
    return _id;
}

std::string Stream::host() const{
    return _host;
}

std::string Stream::port() const{
    return _port;
}

std::string Stream::target() const{
    return _target;
}

boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>& Stream::getSocketSSL(){
    return *_socketSSL;
}

boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& Stream::getSocket(){
    return *_socket;
}

bool Stream::usesSSL() const {
    return _usesSSL;
}

void Stream::feedData(const std::string& data) const{
    if(_cb)
        _cb(true, data);
}

void Stream::connectionAborted(boost::system::error_code ec) const{
    if(_cb)
        _cb(false, ec.message());
}

void Stream::stop() {
    if(_usesSSL){
        if(_socketSSL->is_open()) {
            _socketSSL->async_close(boost::beast::websocket::close_code::normal, [&](boost::system::error_code ec) {
                if (_cb)
                    _cb(false, "Stream stopped by user!");
            });
        }

        return;
    }

    if(_socket->is_open()) {
        _socket->async_close(boost::beast::websocket::close_code::normal, [&](boost::system::error_code ec) {
            if (_cb)
                _cb(false, "Stream stopped by user!");
        });
    }
}

Stream::~Stream(){
    std::cout << "Destructor stream!" << "\n";
}




}
}
}