//
// Created by Arthur Motelevicz on 16/02/23.
//

#include "ResponseHelper.h"
#include "Logger.h"
#include <boost/preprocessor.hpp>
#include <boost/callable_traits.hpp>
#include <boost/beast/version.hpp>

#include <queue>
#include <type_traits>
#include <thread>

namespace bb::network::rest
{
  bool ResponseHelper::isApiError(const mgutils::JsonValue &json,
                                  const std::string &codeKey,
                                  const std::string &msgKey)
  {

    return (json.hasString(codeKey) && json.hasString(msgKey));
  };

  std::pair<int, std::string>
  ResponseHelper::constructError(const mgutils::JsonValue &json,
                                 const std::string &codeKey,
                                 const std::string &msgKey)
  {

    try {
      auto ec = json.getInt(codeKey).value();
      auto msg = json.getString(msgKey).value();

      if (msg.empty())
      {
        auto vec = json.getArray(msgKey);
        msg = !vec.empty() ? vec[0].asString().value() : "";
      }

      return std::make_pair(ec, std::move(msg));

    } catch (const std::bad_optional_access& e) {
      LOG_ERROR(std::string("Error: Missing value in JSON. Exception: ") + e.what());
    }

    return std::make_pair(-1,"");
  }

  bool ResponseHelper::is_valid_value(const valType &v)
  {
    if (const auto *p = boost::get<const char *>(&v))
    {
      return *p != nullptr;
    }
    if (const auto *p = boost::get<std::size_t>(&v))
    {
      return *p != 0u;
    }

    assert(!"unreachable");
    return false;
  }

  const char *ResponseHelper::to_string(char *buf, std::size_t bufsize, const valType &v)
  {
    if (const auto *p = boost::get<const char *>(&v))
    {
      return *p;
    }

    if (const auto *p = boost::get<std::size_t>(&v))
    {
      std::snprintf(buf, bufsize, "%zu", *p);

      return buf;
    }

    assert(!"unreachable");
    return buf;
  }

  bool ResponseHelper::is_html(const char *str)
  {
    return std::strstr(str, "<HTML>") || std::strstr(str, "<HEAD>") || std::strstr(str, "<BODY>");
  }

  bool ResponseHelper::isXML(const std::string &data)
  {
      return data.find("<?xml") != std::string::npos;
  }
}
/*************************************************************************************/