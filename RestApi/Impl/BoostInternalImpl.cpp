//
// Created by Arthur Motelevicz on 17/02/23.
//

#include "BoostInternalImpl.h"
#include "Logger.h"

namespace bb::network::rest
{
  BoostInternalImpl::BoostInternalImpl(boost::asio::io_context &ioctx,
                                       TaskExecutionType executionType,
                                       std::string port,
                                       std::size_t timeout,
                                       std::string client_api_string) :
  m_ioctx{ioctx},
  m_executionType(executionType),
  m_port{std::move(port)},
  m_timeout{timeout},
  m_client_api_string{std::move(client_api_string)},
  m_write_in_process{},
  m_async_requests{},
  m_ssl_ctx{boost::asio::ssl::context::sslv23_client},
  m_resolver{m_ioctx}
  {
    _responseHelper = std::make_unique<ResponseHelper>();
  }

  ResponseHelper *BoostInternalImpl::getResponseHelper()
  {
      return _responseHelper.get();
  }

  NetworkResponse BoostInternalImpl::syncHttpPost(const NetworkRequestSettings &settings,
                                              boost::beast::http::verb action,
                                              std::string data)
  {
    NetworkResponse res{};

    boost::asio::ip::tcp::socket socket(m_ioctx);

    std::string host = settings.getHost();

    boost::system::error_code ec;
    auto const results = m_resolver.resolve(host, m_port, ec);
    if (ec)
    {
      REPORT_ASIO_ERROR_(ec)
      return res;
    }

    boost::asio::connect(socket, results.begin(), results.end(), ec);
    if (ec)
    {
      REPORT_ASIO_ERROR_(ec)
      return res;
    }

    boost::beast::http::request<boost::beast::http::string_body> req;
    req.target(settings.getEndPoint().c_str());
    req.version(11);
    req.method(action);

    if (action != boost::beast::http::verb::get)
    {
      req.body() = std::move(data);
      req.set(boost::beast::http::field::content_length, std::to_string(req.body().length()));
    }

    req.set(boost::beast::http::field::host, host);
    req.set(boost::beast::http::field::user_agent, m_client_api_string);
    req.set(boost::beast::http::field::content_type, settings.getStringContentType());

    auto map = settings.getHeaderMap();
    for (auto &item : map)
    {
      if (std::holds_alternative<std::string>(item.first))
        req.set(std::get<std::string>(item.first), item.second);
      else
        req.set(std::get<HeaderType>(item.first), item.second);
    }

    boost::beast::http::write(socket, req, ec);
    if (ec)
    {
      REPORT_ASIO_ERROR_(ec)
      return res;
    }

    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::string_body> bres;

    boost::beast::http::read(socket, buffer, bres, ec);
    if (ec)
    {
      REPORT_ASIO_ERROR_(ec)
      return res;
    }

    res.http_result_code = static_cast<int>(bres.result_int());
    res.data = std::move(bres.body());

    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if (ec && ec != boost::system::errc::not_connected)
    {
      REPORT_ASIO_ERROR_(ec)
    }

    return res;
  }


  NetworkResponse BoostInternalImpl::syncHttpsPost(const NetworkRequestSettings &settings,
                                              boost::beast::http::verb action,
                                              std::string data)
  {
    NetworkResponse res{};

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_stream(m_ioctx, m_ssl_ctx);

    std::string host = settings.getHost();

    if (!SSL_set_tlsext_host_name(ssl_stream.native_handle(), host.c_str()))
    {
      boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
      lg(mgutils::Error) << __MESSAGE("msg=" + ec.message()) ;
      __MAKE_ERRMSG(res, ec.message());
      return res;
    }

    boost::system::error_code ec;
    auto const results = m_resolver.resolve(host, m_port, ec);
    if (ec)
    {
      lg(mgutils::Error) << __MESSAGE("msg=" << ec.message());
      __MAKE_ERRMSG(res, ec.message());
      return res;
    }

    boost::asio::connect(ssl_stream.next_layer(), results.begin(), results.end(), ec);
    if (ec)
    {
      lg(mgutils::Error) << __MESSAGE("msg=" << ec.message());
        __MAKE_ERRMSG(res, ec.message());
        return res;
    }

    ssl_stream.handshake(boost::asio::ssl::stream_base::client, ec);
    if (ec)
    {
      lg(mgutils::Error) << __MESSAGE("msg=" << ec.message());
      __MAKE_ERRMSG(res, ec.message());
      return res;
    }

    boost::beast::http::request<boost::beast::http::string_body> req;
    req.target(settings.getEndPoint().c_str());
    req.version(11);

    req.method(action);
    if (action != boost::beast::http::verb::get)
    {
      req.body() = std::move(data);
      req.set(boost::beast::http::field::content_length, std::to_string(req.body().length()));
    }

//        req.insert("X-MBX-APIKEY", m_pk);
    req.set(boost::beast::http::field::host, host);
    req.set(boost::beast::http::field::user_agent, m_client_api_string);
    req.set(boost::beast::http::field::content_type, settings.getStringContentType());

    auto map = settings.getHeaderMap();
    for (auto &item: map)
    {
      if (std::holds_alternative<std::string>(item.first))
        req.set(std::get<std::string>(item.first), item.second);
      else
        req.set(std::get<HeaderType>(item.first), item.second);
    }

    boost::beast::http::write(ssl_stream, req, ec);
    if (ec)
    {
      lg(mgutils::Error) << __MESSAGE("msg=" << ec.message()) ;
      __MAKE_ERRMSG(res, ec.message());
      return res;
    }

    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::string_body> bres;

    boost::beast::http::read(ssl_stream, buffer, bres, ec);
    if (ec)
    {
      lg(mgutils::Error) << __MESSAGE("msg=" << ec.message());

      __MAKE_ERRMSG(res, ec.message());
      return res;
    }

    res.http_result_code = static_cast<int>(bres.result_int());
    res.data = std::move(bres.body());

    ssl_stream.shutdown(ec);

    return res;
  }


  //example method not using ssl
  NetworkResponse BoostInternalImpl::sendHttpRequest(const NetworkRequestSettings &settings,
                                                               boost::beast::http::verb action,
                                                               std::string data)
  {
    NetworkResponse res{};

    // These objects perform our I/O
    boost::asio::ip::tcp::resolver resolver{m_ioctx};
    boost::asio::ip::tcp::socket socket{m_ioctx};

    // Look up the domain name
    std::string host = settings.getHost();
    auto const results = resolver.resolve(host, m_port);

    // Make the connection on the IP address we get from a lookup
    boost::asio::connect(socket, results.begin(), results.end());

    // Set up an HTTP GET request message
    boost::beast::http::request<boost::beast::http::string_body> req{action,
                                                                     settings.getEndPoint().c_str(),
                                                                     11};
    req.set(boost::beast::http::field::host, host);
    req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(boost::beast::http::field::content_type, settings.getStringContentType());

    auto map = settings.getHeaderMap();
    for (auto &item: map)
    {
      if (std::holds_alternative<std::string>(item.first))
        req.set(std::get<std::string>(item.first), item.second);
      else
        req.set(std::get<HeaderType>(item.first), item.second);
    }

    if (action != boost::beast::http::verb::get)
    {
      req.body() = std::move(data);
      req.set(boost::beast::http::field::content_length, std::to_string(req.body().length()));
    }

    // Send the HTTP request to the remote host
    boost::beast::http::write(socket, req);

    // This buffer is used for reading and must be persisted
    boost::beast::flat_buffer buffer;

    // Declare a container to hold the response
    boost::beast::http::response<boost::beast::http::string_body> bres;

    // Receive the HTTP response
    boost::beast::http::read(socket, buffer, bres);

    res.data = std::move(bres.body());

    // Write the message to standard out
    lg(mgutils::Info) << bres ;

    // Gracefully close the socket
    boost::system::error_code ec;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    // so don't bother reporting it.
    if (ec && ec != boost::system::errc::not_connected)
      throw boost::system::system_error{ec};


    return res;
  }

  void BoostInternalImpl::asyncPost()
  {
      if (m_write_in_process)
      {
          return;
      }

      m_write_in_process = true;

      auto &front = m_async_requests.front();
      auto action = front.action;
      std::string data = std::move(front.data);
      std::string target = front.settings.getEndPoint();
      //lg(mgutils::Info) << "async_post(): target=" << target ;

      auto req = std::make_unique<request_type>();
      req->version(11);
      req->method(action);
      if (action != boost::beast::http::verb::get)
      {
          req->body() = std::move(data);
          req->set(boost::beast::http::field::content_length, std::to_string(req->body().length()));
      }

      std::string host = front.settings.getHost();
      req->target(target);
  //        req->insert("X-MBX-APIKEY", m_pk);
      req->set(boost::beast::http::field::host, host);
      req->set(boost::beast::http::field::user_agent, m_client_api_string);
      req->set(boost::beast::http::field::content_type, front.settings.getStringContentType());

      auto map = front.settings.getHeaderMap();
      for (auto &item: map)
      {
        if (std::holds_alternative<std::string>(item.first))
          req->set(std::get<std::string>(item.first), item.second);
        else
          req->set(std::get<HeaderType>(item.first), item.second);
      }

      //lg(mgutils::Info) << target << " REQUEST:\n" << m_req ;

      // Look up the domain name
      m_resolver.async_resolve(
        host,
        m_port,
        [this, host, req = std::move(req)] (const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::results_type res) mutable
        {
          onResolve(host, ec, std::move(req), std::move(res));
        }
      );
  }

  void BoostInternalImpl::onResolve(const std::string host,
                                    const boost::system::error_code &ec,
                                    request_ptr req,
                                    boost::asio::ip::tcp::resolver::results_type results) {

      if (ec) {
          m_write_in_process = false;
          processReply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
          return;
      }

      ssl_socket_ptr ssl_socket = std::make_unique<ssl_socket_type>(m_ioctx, m_ssl_ctx);

      if (!SSL_set_tlsext_host_name(ssl_socket->native_handle(), host.c_str())) {
          boost::system::error_code ec2{static_cast<int>(::ERR_get_error()),
                                        boost::asio::error::get_ssl_category()};
        lg(mgutils::Error) << __MESSAGE("msg=" << ec2.message()) ;

          return;
      }

      auto sptr = ssl_socket.get();

      boost::asio::async_connect(
              sptr->next_layer(), results.begin(), results.end(),
              [this, req = std::move(req), ssl_socket = std::move(ssl_socket)]
                      (const boost::system::error_code &ec, auto) mutable {
                  onConnect(ec, std::move(req), std::move(ssl_socket));
              }
      );
  //        boost::asio::await_operation(std::chrono::seconds(6));
  }

  void BoostInternalImpl::onConnect(const boost::system::error_code &ec,
                                    request_ptr req,
                                    ssl_socket_ptr ssl_socket) {

      if (ec) {
          m_write_in_process = false;
          processReply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
          return;
      }

      auto sptr = ssl_socket.get();

      // Perform the SSL handshake
      sptr->async_handshake(
              boost::asio::ssl::stream_base::client,
              [this, req = std::move(req), ssl_socket = std::move(ssl_socket)]
                      (const boost::system::error_code &ec) mutable {
                  onHandshake(ec, std::move(req), std::move(ssl_socket));
              }
      );
  }

  void BoostInternalImpl::onHandshake(const boost::system::error_code &ec,
                                      request_ptr req,
                                      ssl_socket_ptr ssl_socket) {
      if (ec) {
          m_write_in_process = false;
          processReply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
          return;
      }

      auto *request_ptr = req.get();
      auto *socket_ptr = ssl_socket.get();

      // Send the HTTP request to the remote host
      boost::beast::http::async_write(
              *socket_ptr, *request_ptr, [this, req = std::move(req), ssl_socket = std::move(ssl_socket)]
                      (const boost::system::error_code &ec, std::size_t wr) mutable {
                  onWrite(ec, std::move(req), std::move(ssl_socket), wr);
              }
      );
  }

  void BoostInternalImpl::onWrite(const boost::system::error_code &ec,
                                  request_ptr req,
                                  ssl_socket_ptr ssl_socket,
                                  std::size_t wr) {
      boost::ignore_unused(wr);
      boost::ignore_unused(req);

      if (ec) {
          m_write_in_process = false;
          processReply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
          return;
      }

      auto resp = std::make_unique<response_type>();
      auto *resp_ptr = resp.get();
      auto *socket_ptr = ssl_socket.get();

      // Receive the HTTP response
      boost::beast::http::async_read(
              *socket_ptr, m_buffer, *resp_ptr,
              [this, resp = std::move(resp), ssl_socket = std::move(ssl_socket)]
                      (const boost::system::error_code &ec, std::size_t rd) mutable {
                  onRead(ec, std::move(resp), std::move(ssl_socket), rd);
              }
      );
  }

  void BoostInternalImpl::onRead(const boost::system::error_code &ec,
                                 response_ptr resp,
                                 ssl_socket_ptr ssl_socket,
                                 std::size_t rd) {
      boost::ignore_unused(rd);

      if (ec) {
          m_write_in_process = false;
          processReply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
          return;
      }

      auto *socket_ptr = ssl_socket.get();

      socket_ptr->async_shutdown(
              [this, resp = std::move(resp), ssl_socket = std::move(ssl_socket)]
                      (const boost::system::error_code &ec) mutable {
                  onShutdown(ec, std::move(resp), std::move(ssl_socket));
              }
      );
  }

  void BoostInternalImpl::onShutdown(const boost::system::error_code &ec,
                                     response_ptr resp,
                                     ssl_socket_ptr ssl_socket) {
      boost::ignore_unused(ec);
      boost::ignore_unused(ssl_socket);

      int httpRespCode = resp->result_int();
      std::string body = std::move(resp->body());
      processReply(__MAKE_FILELINE, 0, std::string{}, std::move(body), httpRespCode);

      m_write_in_process = false;

      if (!m_async_requests.empty()) {
          asyncPost();
      }
  }

  void BoostInternalImpl::processReply(const char *fl,
                                       int ec,
                                       std::string errmsg,
                                       std::string body,
                                       int http_result_code)
  {
    assert(!m_async_requests.empty());
    try
    {
      const auto item = std::move(m_async_requests.front());
      m_async_requests.pop();

      //lg(mgutils::Info) << "process_reply(): target=" << item.target ;
      item.invoker->invoke(fl, ec, std::move(errmsg), body, http_result_code);
    }
    catch (std::exception &e)
    {
      lg(mgutils::Error) << e.what();
    }
  }
}