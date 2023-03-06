//
// Created by Arthur Motelevicz on 16/02/23.
//

#include "NetworkRequestSettings.h"
#include <openssl/evp.h>
#include <openssl/hmac.h>
namespace bb {
    namespace network {
        namespace rest {

//Criar classe para essas duas funções
            std::string b2aHex(const std::uint8_t *p, std::size_t n) {
                static const char hex[] = "0123456789abcdef";
                std::string res;
                res.reserve(n * 2);

                for (auto end = p + n; p != end; ++p) {
                    const std::uint8_t v = (*p);
                    res += hex[(v >> 4) & 0x0F];
                    res += hex[v & 0x0F];
                }

                return res;
            }

            std::string hmacSha256(const char *key, std::size_t klen, const char *data, std::size_t dlen) {
                std::uint8_t digest[EVP_MAX_MD_SIZE];
                std::uint32_t dilen{};

                auto p = ::HMAC(
                        ::EVP_sha256(), key, klen, (std::uint8_t *) data, dlen, digest, &dilen
                );
                assert(p);

                return b2aHex(digest, dilen);
            }

            NetworkRequestSettings &NetworkRequestSettings::setHost(const std::string &url) {
                _host = url;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setEndPoint(const std::string &endPoint) {
                _endPoint = endPoint;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setContentType(const ContentType contentType) {
                _contentType = contentType;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setApiKeyType(const std::string &apiKeyType) {
                _apiKeyType = apiKeyType;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setApiKey(const std::string &apiKey) {
                _apiKey = apiKey;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setTaskExecutionType(const TaskExecutionType type) {
                _taskExecutionType = type;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setFullUrl(const std::string url) {
                std::string res = url;
                auto id = res.find("//");
                if (id != std::string::npos) {
                    res = res.substr(id + 2);
                }

                id = res.find("/");
                if (id != std::string::npos) {
                    _endPoint = res.substr(id);
                    res = res.substr(0, id);
                }

                _host = res;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setBody(const std::string &data) {
                _body = data;

                return (*this);
            }

            NetworkRequestSettings &NetworkRequestSettings::setBody(const ListData &list) {
                std::string data;
                for (const auto &it: list) {
                    if (!data.empty()) {
                        data += "&";
                    }
                    data += it.first;
                    data += "=";

//        char buf[32];
//        data += to_string(buf, sizeof(buf), it.second);
                    data += it.second;
                }

                _body = data;

                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setBody(const rapidjson::Document &document) {
                return setBody(bb::Json::toString(document));
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setApiKey(const std::string &key, const std::string &value) {
                _kapiKey = key;
                _apiKey = value;
                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::setSecurityKey(const std::string &key, const std::string &value) {
                _ksecurityKey = key;
                _securityKey = value;
                return (*this);
            }

            NetworkRequestSettings &
            NetworkRequestSettings::addHeaderValue(const HeaderVariant key, const std::string &value) {
                _headerMap.insert_or_assign(key, value);

                return (*this);
            }

            std::string NetworkRequestSettings::body() {

                if (!_securityKey.empty()) {
                    if (!_body.empty())
                        _body += "&";

                    std::string signature = hmacSha256(
                            _securityKey.c_str(),
                            _securityKey.length(),
                            _body.c_str(),
                            _body.length()
                    );
                    _body += _ksecurityKey + "=";
                    _body += signature;
                }

                return _body;
            }

            std::string NetworkRequestSettings::getStringContentType() const {
                switch (_contentType) {
                    case ContentType::FORM:
                        return "application/x-www-form-urlencoded";
                    case ContentType::JSON:
                        return "application/json";
                }

                return "";
            }

            ContentType NetworkRequestSettings::getContentType() const {
                return _contentType;
            }

            std::string NetworkRequestSettings::getHost() const {
                return _host;
            }

            std::string NetworkRequestSettings::getEndPoint() const {
                return _endPoint;
            }

            std::string NetworkRequestSettings::getApiKey() const {
                return _apiKey;
            }

            std::string NetworkRequestSettings::getKeyApiKey() const {
                return _kapiKey;
            }

            std::string NetworkRequestSettings::getApiKeyType() const {
                return _apiKeyType;
            }

            TaskExecutionType NetworkRequestSettings::getTaskExecutionType() const {
                return _taskExecutionType;
            }

            const std::map<HeaderVariant, std::string> &
            NetworkRequestSettings::getHeaderMap() const {
                return _headerMap;
            }

        }//ns network
    }//ns bb
}