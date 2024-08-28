//
// Created by Arthur Motelevicz on 06/03/23.
//

#include <iostream>
#include <beastboys>

void signin()
{
  std::shared_ptr<bb::network::rest::RestApi> api{nullptr};
  api = std::make_shared<bb::network::rest::RestApi>("3000", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);

  auto jsonString = R"({"email":"leviczios@gmail.com", "password":"Tardatordo3*"})";

  if (auto document = mgutils::Json::parse(jsonString))
  {
    assert(document->getRoot().hasString("email"));
    assert(document->getRoot().hasString("password"));

    auto settings = bb::network::rest::NetworkRequestSettings();
    settings.setFullUrl("http://localhost/auth/sign-in")
        .setContentType(bb::network::rest::ContentType::JSON)
        .setBody(*document);

    api->post(settings, [&](const bb::network::rest::NetworkResponse &response)
    {
      bool success = response.isOk();
      std::string msg = response.message;
      lg(mgutils::Info) << msg;
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
  signin();
    return EXIT_SUCCESS;
}