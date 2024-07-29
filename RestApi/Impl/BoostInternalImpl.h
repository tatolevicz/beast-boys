//
// Created by Arthur Motelevicz on 17/02/23.
//

#ifndef BROKERAPP_BOOSTINTERNALIMPL_H
#define BROKERAPP_BOOSTINTERNALIMPL_H

#include "NetworkRequestSettings.h"
#include "ResponseHelper.h"

#include <boost/beast.hpp>
#include <type_traits>
#include <iostream>
#include <thread>
#include <queue>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/preprocessor.hpp>
#include <boost/callable_traits.hpp>
#include <boost/variant.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace bb::network::rest
{

class BoostInternalImpl
{
public:
  BoostInternalImpl(boost::asio::io_context &ioctx,
                    TaskExecutionType executionType,
                    std::string port,
                    std::size_t timeout,
                    std::string client_api_string = "botb_boost_beast_api_impl-0.0.1");

  template<typename CB,
          typename Args = typename boost::callable_traits::args<CB>::type,
          typename R = typename std::tuple_element<3, Args>::type
  >
  NetworkResponse post(NetworkRequestSettings &settings,
                       boost::beast::http::verb action,
                       CB cb,
                       bool isHttps = true)
  {
    static_assert(std::tuple_size<Args>::value == 5, "callback signature is wrong!");

    m_useTLS = settings.isHttps();

    std::string starget = settings.getEndPoint();
    std::string data = settings.body();
    bool getOrdelete = (action == boost::beast::http::verb::get ||
                        action == boost::beast::http::verb::delete_);
    if (getOrdelete && !data.empty())
    {
      assert(settings.getContentType() == ContentType::FORM);
      starget += "?";
      starget += data;
      data.clear();
      settings.setEndPoint(starget);
    }

    NetworkResponse r{};
    if (m_executionType == TaskExecutionType::BB_SYNCH)
    {
      try
      {
        if(m_useTLS)
          r = syncHttpsPost(settings, action, std::move(data));
        else
          r = syncHttpPost(settings, action, std::move(data));

        if (!r.data.empty() && _responseHelper->is_html(r.data.c_str()))
        {
          r.message = std::move(r.data);
        }
      }
      catch (const std::exception &ex)
      {
        __MAKE_ERRMSG(r, ex.what())
      }

      return r;
    }
    else
    {
      using invoker_type = invoker<typename boost::callable_traits::return_type<CB>::type, R, CB>;
      async_req_item item
      {
        settings,
        action,
        std::move(data),
        std::make_shared<invoker_type>(std::move(cb))
      };

      std::lock_guard<std::mutex> lg(_insertItemMutex);

      m_async_requests.push(std::move(item));

      asyncPost();
    }

    return r;
  }

  NetworkResponse syncHttpsPost(const NetworkRequestSettings &settings,
                           boost::beast::http::verb action,
                           std::string data);

  NetworkResponse syncHttpPost(const NetworkRequestSettings &settings,
                                boost::beast::http::verb action,
                                std::string data);

  //example method not using ssl (not used in the project but can be util in the future)
  NetworkResponse sendHttpRequest(const NetworkRequestSettings &settings,
                  boost::beast::http::verb action,
                  std::string data);

  using request_ptr = std::unique_ptr<boost::beast::http::request<boost::beast::http::string_body>>;
  using request_type = typename request_ptr::element_type;
  using response_ptr = std::unique_ptr<boost::beast::http::response<boost::beast::http::string_body>>;
  using response_type = typename response_ptr::element_type;
  using ssl_socket_ptr = std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>;
  using ssl_socket_type = typename ssl_socket_ptr::element_type;

  void asyncPost();

  void onResolve(const std::string host,
                 const boost::system::error_code &ec,
                 request_ptr req,
                 boost::asio::ip::tcp::resolver::results_type results);

  void onConnect(const boost::system::error_code &ec,
                 request_ptr req,
                 ssl_socket_ptr ssl_socket);

  void onHandshake(const boost::system::error_code &ec,
                   request_ptr req,
                   ssl_socket_ptr ssl_socket);

  void onWrite(const boost::system::error_code &ec,
               request_ptr req,
               ssl_socket_ptr ssl_socket,
               std::size_t wr);

  void onRead(const boost::system::error_code &ec,
              response_ptr resp,
              ssl_socket_ptr ssl_socket,
              std::size_t rd);

  void onShutdown(const boost::system::error_code &ec, response_ptr resp, ssl_socket_ptr ssl_socket);

  void processReply(const char *fl, int ec, std::string errmsg, std::string body, int http_result_code = -1);

  ResponseHelper *getResponseHelper();

private:
  std::unique_ptr<ResponseHelper> _responseHelper;
  boost::asio::io_context &m_ioctx;
  const std::string m_port;
  const std::size_t m_timeout;
  const std::string m_client_api_string;

  bool m_write_in_process;
  bool _isHttps{true};
  struct async_req_item {
      NetworkRequestSettings settings;
      boost::beast::http::verb action;
      std::string data;
      invoker_ptr invoker;
  };

  std::queue<async_req_item> m_async_requests;
  boost::asio::ssl::context m_ssl_ctx;
  boost::asio::ip::tcp::resolver m_resolver;
  boost::beast::flat_buffer m_buffer;  // (Must persist between reads)
  TaskExecutionType m_executionType;
  bool m_useTLS{true};
  std::mutex _insertItemMutex;
};

}
#endif //BROKERAPP_BOOSTINTERNALIMPL_H
