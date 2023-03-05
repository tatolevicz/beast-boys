//
// Created by Arthur Motelevicz on 16/02/23.
//

#include "ResponseHelper.h"

#include <boost/preprocessor.hpp>
#include <boost/callable_traits.hpp>
#include <boost/beast/version.hpp>

#include <queue>
#include <type_traits>
#include <thread>


namespace bb {
    namespace network {
        namespace rest {


            bool ResponseHelper::isApiError(const rapidjson::Value &json, const std::string &codeKey,
                                            const std::string &msgKey) {
                return (json.HasMember(codeKey.c_str()) && json.HasMember(msgKey.c_str()));
            };

            std::pair<int, std::string>
            ResponseHelper::constructError(const rapidjson::Value &json, const std::string &codeKey,
                                           const std::string &msgKey) {

                auto ec = Json::getInt(json, codeKey.c_str());
                auto msg = Json::getString(json, msgKey.c_str());

                if (msg.empty()) {
                    auto vec = Json::getStringVector(json, msgKey.c_str());
                    msg = !vec.empty() ? vec[0] : "";
                }

                return std::make_pair(ec, std::move(msg));
            }

            bool ResponseHelper::is_valid_value(const valType &v) {
                if (const auto *p = boost::get<const char *>(&v)) {
                    return *p != nullptr;
                }
                if (const auto *p = boost::get<std::size_t>(&v)) {
                    return *p != 0u;
                }

                assert(!"unreachable");

                return false;
            }

            const char *ResponseHelper::to_string(char *buf, std::size_t bufsize, const valType &v) {
                if (const auto *p = boost::get<const char *>(&v)) {
                    return *p;
                }
                if (const auto *p = boost::get<std::size_t>(&v)) {
                    std::snprintf(buf, bufsize, "%zu", *p);

                    return buf;
                }

                assert(!"unreachable");

                return buf;
            }

            bool ResponseHelper::is_html(const char *str) {
                return std::strstr(str, "<HTML>") || std::strstr(str, "<HEAD>") || std::strstr(str, "<BODY>");
            }

            bool ResponseHelper::isXML(const std::string &data) {
                return data.find("<?xml") != std::string::npos;
            }
        }
    }
}
/*************************************************************************************/