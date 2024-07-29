//
// Created by Arthur Motelevicz on 29/07/24.
//

#include <beastboys>
#include <thread>
#include <string>
#include "catch2/catch.hpp"
#include "TestHelpers.h"

//TEST_CASE("RestApi sign-in test") {
//  std::shared_ptr<bb::network::rest::RestApi> api =
//      std::make_shared<bb::network::rest::RestApi>("3000", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
//  auto document = bb::Json::document();
//  auto jsonString = R"({"email":"leviczios@gmail.com", "password":"Tardatordo3*"})";
//
//  REQUIRE(bb::Json::parse(jsonString, document));
//
//  REQUIRE(document.HasMember("email"));
//  REQUIRE(document.HasMember("password"));
//
//  auto settings = bb::network::rest::NetworkRequestSettings();
//  settings.setFullUrl("http://localhost/auth/sign-in")
//      .setContentType(bb::network::rest::ContentType::JSON)
//      .setBody(document);
//
//  SECTION("Valid sign-in") {
//    api->post(settings, [&](const bb::network::rest::NetworkResponse &response)
//    {
//      bool success = response.isOk();
//      std::string msg = response.message;
//      std::cout << msg << "\n";
//      REQUIRE(success == true);
//      REQUIRE(response.http_result_code == 200);
//    });
//  }
//
//  SECTION("Invalid sign-in") {
//    // Modify the document for an invalid request
//    document["password"].SetString("wrongpassword", document.GetAllocator());
//    settings.setBody(document);
//
//    api->post(settings, [&](const bb::network::rest::NetworkResponse &response) {
//      bool success = response.isOk();
//      std::string msg = response.message;
//      std::cout << msg << "\n";
//      REQUIRE(success == false);
//      REQUIRE(response.http_result_code == 401);  // Assuming 401 is the code for unauthorized
//    });
//  }
//}


TEST_CASE("RestApi HTTP and HTTPS test") {
  auto api = std::make_shared<bb::network::rest::RestApi>("80", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
  auto document = bb::Json::document();

  SECTION("HTTP GET request to public endpoint") {
    auto settings = bb::network::rest::NetworkRequestSettings();
    settings.setFullUrl("http://jsonplaceholder.typicode.com/posts/1")
        .setContentType(bb::network::rest::ContentType::JSON);

    api->get(settings, [&](const bb::network::rest::NetworkResponse &response) {
      bool success = response.isOk();
      std::string msg = response.message;
      std::cout << msg << "\n";
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 200);
      REQUIRE(!response.data.empty());
    });
  }

  SECTION("HTTPS GET request to public endpoint") {
    auto api_https = std::make_shared<bb::network::rest::RestApi>("443", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
    auto settings = bb::network::rest::NetworkRequestSettings();
    settings.setFullUrl("https://jsonplaceholder.typicode.com/posts/1")
        .setContentType(bb::network::rest::ContentType::JSON);

    api_https->get(settings, [&](const bb::network::rest::NetworkResponse &response)
    {
      bool success = response.isOk();
      std::string msg = response.message;
      std::cout << msg << "\n";
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 200);
      REQUIRE(!response.data.empty());
    });
  }

  SECTION("HTTP POST request to public endpoint") {
    auto settings = bb::network::rest::NetworkRequestSettings();
    settings.setFullUrl("http://jsonplaceholder.typicode.com/posts")
        .setContentType(bb::network::rest::ContentType::JSON);

    std::string postData = R"({"title": "foo", "body": "bar", "userId": 1})";
    document.Parse(postData.c_str());
    settings.setBody(document);

    api->post(settings, [&](const bb::network::rest::NetworkResponse &response) {
      bool success = response.isOk();
      std::string msg = response.message;
      std::cout << msg << "\n";
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 201);
      REQUIRE(!response.data.empty());
    });
  }

  SECTION("HTTPS POST request to public endpoint") {
    auto api_https = std::make_shared<bb::network::rest::RestApi>("443", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
    auto settings = bb::network::rest::NetworkRequestSettings();
    settings.setFullUrl("https://jsonplaceholder.typicode.com/posts")
        .setContentType(bb::network::rest::ContentType::JSON);

    std::string postData = R"({"title": "foo", "body": "bar", "userId": 1})";
    document.Parse(postData.c_str());
    settings.setBody(document);

    api_https->post(settings, [&](const bb::network::rest::NetworkResponse &response)
    {
      bool success = response.isOk();
      std::string msg = response.message;
      std::cout << msg << "\n";
      REQUIRE(success == true);
      REQUIRE(response.http_result_code == 201);
      REQUIRE(!response.data.empty());
    });
  }
}