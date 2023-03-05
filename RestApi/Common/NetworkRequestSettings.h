//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BROKERAPP_NETWORKREQUESTSETTINGS_H
#define BROKERAPP_NETWORKREQUESTSETTINGS_H

#include <string>
#include "NetworkTypes.h"
#include <map>

namespace bb {
    namespace network {
        namespace rest {
            class NetworkRequestSettings {
            public:
                NetworkRequestSettings &setHost(const std::string &url);

                NetworkRequestSettings &setEndPoint(const std::string &endPoint);

                NetworkRequestSettings &setContentType(const ContentType contentType);

                NetworkRequestSettings &setApiKeyType(const std::string &apiKeyType);

                NetworkRequestSettings &setApiKey(const std::string &apiKey);

                NetworkRequestSettings &setTaskExecutionType(const TaskExecutionType type);

                NetworkRequestSettings &setFullUrl(const std::string url);

                NetworkRequestSettings &setBody(const std::string &data);

                NetworkRequestSettings &setBody(const ListData &list);

                NetworkRequestSettings &setBody(const rapidjson::Document &document);

                NetworkRequestSettings &setApiKey(const std::string &key, const std::string &value);

                NetworkRequestSettings &setSecurityKey(const std::string &key, const std::string &value);

                NetworkRequestSettings &addHeaderValue(const HeaderVariant key, const std::string &value);

                std::string body();

                std::string getStringContentType() const;

                ContentType getContentType() const;

                std::string getHost() const;

                std::string getEndPoint() const;

                std::string getApiKey() const;

                std::string getKeyApiKey() const;

                std::string getApiKeyType() const;

                TaskExecutionType getTaskExecutionType() const;

                const std::map<HeaderVariant, std::string> &getHeaderMap() const;

            private:
                ContentType _contentType{ContentType::FORM};
                std::string _host;
                std::string _endPoint;
                std::string _apiKeyType;
                std::string _body;
                std::string _apiKey;
                std::string _securityKey;
                std::string _kapiKey;
                std::string _ksecurityKey;
                TaskExecutionType _taskExecutionType{TaskExecutionType::ASYNCH};
                std::map<HeaderVariant, std::string> _headerMap;
            };
        }
    }
}

#endif //BROKERAPP_NETWORKREQUESTSETTINGS_H
