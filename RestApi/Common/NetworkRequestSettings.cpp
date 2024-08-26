//
// Created by Arthur Motelevicz on 16/02/23.
//

#include "NetworkRequestSettings.h"

namespace bb::network::rest
{

NetworkRequestSettings &
NetworkRequestSettings::setHost(const std::string &url, bool isHttps)
{
  _host = url;
  _isHttps = isHttps;
  return (*this);
}

NetworkRequestSettings &
NetworkRequestSettings::setEndPoint(const std::string &endPoint)
{
  _endPoint = endPoint;
  return (*this);
}

NetworkRequestSettings &
NetworkRequestSettings::setContentType(const ContentType contentType)
{
  _contentType = contentType;
  return (*this);
}

NetworkRequestSettings &
NetworkRequestSettings::setFullUrl(const std::string url)
{
  assert(url.size() > 5);

  _isHttps = url.substr(0, 5) == "https";

  std::string res = url;
  auto id = res.find("//");
  if (id != std::string::npos)
  {
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
NetworkRequestSettings::setBody(const std::string &data)
{
  _body = data;
  return (*this);
}

NetworkRequestSettings &
NetworkRequestSettings::setBody(const ListData &list)
{
  std::string data;
  for (const auto &it: list)
  {
    if (!data.empty()) {
      data += "&";
    }
    data += it.first;
    data += "=";
    data += it.second;
  }

  _body = data;

  return (*this);
}

NetworkRequestSettings &
NetworkRequestSettings::setBody(const mgutils::JsonDocument &document)
{
  return setBody(document.toString());
}

NetworkRequestSettings &
NetworkRequestSettings::addHeaderValue(const HeaderVariant& key, const std::string &value)
{
  _headerMap.insert_or_assign(key, value);
  return (*this);
}

std::string NetworkRequestSettings::body()
{
  return _body;
}

std::string NetworkRequestSettings::getStringContentType() const
{
  switch (_contentType) {
    case ContentType::FORM:
      return "application/x-www-form-urlencoded";
    case ContentType::JSON:
      return "application/json";
  }

  return "";
}

ContentType NetworkRequestSettings::getContentType() const
{
  return _contentType;
}

std::string NetworkRequestSettings::getHost() const
{
  return _host;
}

std::string NetworkRequestSettings::getEndPoint() const
{
  return _endPoint;
}

const std::map<HeaderVariant, std::string> &
NetworkRequestSettings::getHeaderMap() const
{
  return _headerMap;
}

bool NetworkRequestSettings::isHttps() const
{
  return _isHttps;
}

}