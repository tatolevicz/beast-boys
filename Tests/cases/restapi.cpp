//
// Created by Arthur Motelevicz on 29/07/24.
//

#include <beastboys>
#include <thread>
#include <string>
#include "catch2/catch.hpp"

TEST_CASE("RestApi HTTP test","[rest][http]")
{
  auto api = std::make_shared<bb::network::rest::RestApi>("80", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
  auto settings = bb::network::rest::NetworkRequestSettings();
  auto document = mgutils::Json::createDocument(mgutils::JsonRootType::OBJECT);

  SECTION("HTTP GET request to public endpoint")
  {
    settings.setFullUrl("http://jsonplaceholder.typicode.com/posts/1").setContentType(
        bb::network::rest::ContentType::JSON);

    api->get(settings, [&](const bb::network::rest::NetworkResponse &response)
    {
      bool success = response.isOk();
      std::string msg = response.message;
      lg(mgutils::Info) << msg;
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 200);
      REQUIRE(!response.data.empty());
    });
  }

  SECTION("HTTP POST request to public endpoint")
  {
    settings.setFullUrl("http://jsonplaceholder.typicode.com/posts").setContentType(
        bb::network::rest::ContentType::JSON);

    std::string postData = R"({"title": "foo", "body": "bar", "userId": 1})";
    auto doc = mgutils::Json::parse(postData);
    settings.setBody(*document);

    api->post(settings, [&](const bb::network::rest::NetworkResponse &response)
    {
      bool success = response.isOk();
      std::string msg = response.message;
      lg(mgutils::Info) << msg;
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 201);
      REQUIRE(!response.data.empty());
    });
  }
}

TEST_CASE("RestApi HTTPS test","[rest][https]")
{
  auto api_https = std::make_shared<bb::network::rest::RestApi>("443", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
  auto settings = bb::network::rest::NetworkRequestSettings();

  SECTION("HTTPS GET request to public endpoint")
  {
    settings.setFullUrl("https://jsonplaceholder.typicode.com/posts/1").setContentType(
        bb::network::rest::ContentType::JSON);

    api_https->get(settings, [&](const bb::network::rest::NetworkResponse &response)
    {
      bool success = response.isOk();
      std::string msg = response.message;
      lg(mgutils::Info) << msg;
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 200);
      REQUIRE(!response.data.empty());
    });
  }

  SECTION("HTTPS POST request to public endpoint", "[rest][https]")
  {
    settings.setFullUrl("https://jsonplaceholder.typicode.com/posts")
    .setContentType(bb::network::rest::ContentType::JSON);

    std::string postData = R"({"title": "foo", "body": "bar", "userId": 1})";
    auto doc = mgutils::Json::parse(postData);
    settings.setBody(*doc);

    api_https->post(settings, [&](const bb::network::rest::NetworkResponse &response)
    {
      bool success = response.isOk();
      std::string msg = response.message;
      lg(mgutils::Info) << msg;
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 201);
      REQUIRE(!response.data.empty());
    });
  }
}