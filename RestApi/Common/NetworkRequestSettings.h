//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BEASTBOYS_NETWORKREQUESTSETTINGS_H
#define BEASTBOYS_NETWORKREQUESTSETTINGS_H

#include <mgutils/Json.h>
#include <string>
#include "NetworkTypes.h"
#include <map>

namespace bb::network::rest
{
class NetworkRequestSettings
{
public:
  NetworkRequestSettings &setHost(const std::string &url, bool isHttps = true);
  NetworkRequestSettings &setEndPoint(const std::string &endPoint);
  NetworkRequestSettings &setContentType(const ContentType contentType);
  NetworkRequestSettings &setFullUrl(const std::string url);
  NetworkRequestSettings &setBody(const std::string &data);
  NetworkRequestSettings &setBody(const ListData &list);
  NetworkRequestSettings &setBody(const mgutils::JsonDocument &document);
  NetworkRequestSettings &addHeaderValue(const HeaderVariant& key, const std::string &value);

  std::string body();
  std::string getStringContentType() const;
  ContentType getContentType() const;
  std::string getHost() const;
  std::string getEndPoint() const;
  bool isHttps() const;

  const std::map<HeaderVariant, std::string> &getHeaderMap() const;

private:
  ContentType _contentType{ContentType::FORM};
  bool _isHttps = true;
  std::string _host;
  std::string _endPoint;
  std::string _apiKeyType;
  std::string _body;
  std::map<HeaderVariant, std::string> _headerMap;
};

}

#endif //BEASTBOYS_NETWORKREQUESTSETTINGS_H
