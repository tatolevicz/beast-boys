//
// Created by Arthur Motelevicz on 06/03/23.
//

#include <iostream>
#include "NetworkRequestSettings.h"
#include "RestApi.h"

//void exampleGettingUserListenKey(){
//
//    std::string testSpotApiKey = "kovHrnxpU0PsCAGSTnPUvxdBX1ZGYgAqTtmLLniO2aMmTq4i0wwAXgLundvb6fiU";
//    auto _restApi = std::make_unique<bb::network::rest::RestApi>("443",60000);
//
//    std::string resp = "";
//
//    auto settings = bb::network::rest::NetworkRequestSettings();
//
//    settings.addHeaderValue("X-MBX-APIKEY", testSpotApiKey)
//            .setContentType(bb::network::rest::ContentType::FORM)
//            .setFullUrl("https://testnet.binance.vision/api/v3/userDataStream");
//
//    _restApi->post(settings, [](const bb::network::rest::NetworkResponse& response) {
//        if(response.isOk()){
//            std::cout << "Data user: " << response.data <<  "\n";
//        }
//        else{
//            std::cout << "Data user OK error!" << "\n";
//        }
//    });
//}
//

// magia api tests
void signin()
{
  std::shared_ptr<bb::network::rest::RestApi> api{nullptr};
  api = std::make_shared<bb::network::rest::RestApi>("3000", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
  auto documet = bb::Json::document();
  auto jsonString = "{\"email\":\"leviczios@gmail.com\", \"password\":\"Tardatordo3*\"}";

  if (bb::Json::parse(jsonString, documet)) {
    assert(documet.HasMember("email"));
    assert(documet.HasMember("password"));

    auto settings = bb::network::rest::NetworkRequestSettings();
    settings.setFullUrl("https://localhost/auth/sign-in")
        .setContentType(bb::network::rest::ContentType::JSON)
        .setBody(documet);

    api->post(settings, [&](const bb::network::rest::NetworkResponse &response) {
      bool success = response.isOk();
      std::string msg = response.message;
      std::cout << msg << "\n";
//      if (success) {
//        user.reset(UserParser::parse(response.data));
//      }
//      else {
//        msg = getErrorMessage(response.data);
//      }
    });
  }

}

int main() {
//    exampleGettingUserListenKey();
  signin();
    return EXIT_SUCCESS;
}