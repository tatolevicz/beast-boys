//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BEASTBOYS_RESPONSEHELPER_H
#define BEASTBOYS_RESPONSEHELPER_H

#include <mgutils/Json.h>
#include <memory>
#include "NetworkTypes.h"

#define __STRINGIZE_I(x) #x
#define __STRINGIZE(x) __STRINGIZE_I(x)

#define __MAKE_FILELINE \
    __FILE__ "(" __STRINGIZE(__LINE__) ")"

#define __MAKE_ERRMSG(res, msg) \
    res.message = __MAKE_FILELINE; \
    res.message += ": "; \
    res.message += msg;

#define __MESSAGE(msg) \
    std::strrchr(__FILE__, '/')+1 << "(" __STRINGIZE(__LINE__) "): " << msg

#define __CATCH_BLOCK_WRAP_X(...) ((__VA_ARGS__)) __CATCH_BLOCK_WRAP_Y
#define __CATCH_BLOCK_WRAP_Y(...) ((__VA_ARGS__)) __CATCH_BLOCK_WRAP_X
#define __CATCH_BLOCK_WRAP_X0
#define __CATCH_BLOCK_WRAP_Y0

#ifndef __CATCH_BLOCK_WRITES_TO_STDOUT
#   define  __CATCH_BLOCK_WRITES_TO_STDOUT_EXPAND_EXPR(...)
#else
#   define  __CATCH_BLOCK_WRITES_TO_STDOUT_EXPAND_EXPR(...) __VA_ARGS__
#endif // __CATCH_BLOCK_WRITES_TO_STDOUT

#define __CATCH_BLOCK_WITH_USERCODE(os, exception, ...) \
    catch (const exception &ex) { \
        __CATCH_BLOCK_WRITES_TO_STDOUT_EXPAND_EXPR( \
            os << __MESSAGE("[" BOOST_PP_STRINGIZE(exception) "]: " << ex.what()) << std::endl; \
        ) \
        { BOOST_PP_EXPAND __VA_ARGS__; } \
    }

#define __CATCH_BLOCK_CB(unused0, data, elem) \
    BOOST_PP_IF( \
         BOOST_PP_GREATER(BOOST_PP_TUPLE_SIZE(elem), 1) \
        ,__CATCH_BLOCK_WITH_USERCODE \
        ,__CATCH_BLOCK_WITHOUT_USERCODE \
    )( \
         data \
        ,BOOST_PP_TUPLE_ELEM(0, elem) \
        ,BOOST_PP_TUPLE_POP_FRONT(elem) \
    )

#define __CATCH_BLOCK(os, seq) \
    BOOST_PP_SEQ_FOR_EACH( \
         __CATCH_BLOCK_CB \
        ,os \
        ,BOOST_PP_CAT(__CATCH_BLOCK_WRAP_X seq, 0) \
    )

#define __TRY_BLOCK() \
    try



namespace bb::network::rest
{

  class ResponseHelper {
  public:
      bool isApiError(const mgutils::JsonValue &json, const std::string &codeKey, const std::string &msgKey);

      std::pair<int, std::string>
      constructError(const mgutils::JsonValue &json, const std::string &codeKey, const std::string &msgKey);

      bool is_valid_value(const valType &v);

      const char *to_string(char *buf, std::size_t bufsize, const valType &v);

      bool is_html(const char *str);

      bool isXML(const std::string &data);
  };
  /*************************************************************************************************/

  struct invoker_base {
      virtual bool
      invoke(const char *fl, int ec, std::string errmsg, std::string data, int http_result_code) = 0;
  };

  using invoker_ptr = std::shared_ptr<invoker_base>;
/*************************************************************************************************/

  template<typename R, typename T, typename F>
  struct invoker : invoker_base
  {
    invoker(F f) :
    m_cb(std::move(f))
    {
      _responseHelper = std::make_unique<ResponseHelper>();
    }

    virtual ~invoker() = default;

    bool
    invoke(const char *fl, int ec, std::string errmsg, std::string data, int http_result_code) override
    {
        try
        {
          if (data.empty() || ec)
          {
            T arg{};
            return m_cb(fl, ec, std::move(errmsg), std::move(arg), http_result_code);
          }
          else
          {
            if (!data.empty() && _responseHelper->is_html(data.c_str()))
            {
              return m_cb(__MAKE_FILELINE, -1, std::move(errmsg), std::move(data), http_result_code);
            }

            return m_cb(__MAKE_FILELINE, 0, std::move(errmsg), std::move(data), http_result_code);
          }
        }
        catch (const std::exception &ex)
        {
            std::fprintf(stderr, "%s: ex=%s\n", __MAKE_FILELINE, ex.what());
            std::fprintf(stderr, "size=%u, ptr=%s\n", (unsigned) data.size(), data.c_str());
            std::fflush(stderr);
        }
        return true;
    }

    std::unique_ptr<ResponseHelper> _responseHelper{nullptr};
    F m_cb;
  };
}

#endif //BEASTBOYS_RESPONSEHELPER_H
