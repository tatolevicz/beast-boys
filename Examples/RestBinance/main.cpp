//
// Created by Arthur Motelevicz on 06/03/23.
//

#include <iostream>
#include "NetworkRequestSettings.h"
#include "RestApi.h"

void exampleGettingUserListenKey(){

    std::string testSpotApiKey = "kovHrnxpU0PsCAGSTnPUvxdBX1ZGYgAqTtmLLniO2aMmTq4i0wwAXgLundvb6fiU";
    auto _restApi = std::make_unique<bb::network::rest::RestApi>("443",60000);

    std::string resp = "";

    auto settings = bb::network::rest::NetworkRequestSettings();

    settings.addHeaderValue("X-MBX-APIKEY", testSpotApiKey)
            .setContentType(bb::network::rest::ContentType::FORM)
            .setFullUrl("https://testnet.binance.vision/api/v3/userDataStream");

    _restApi->post(settings, [](const bb::network::rest::NetworkResponse& response) {
        if(response.isOk()){
            std::cout << "Data user: " << response.data <<  "\n";
        }
        else{
            std::cout << "Data user OK error!" << "\n";
        }
    });
}

int main() {
    exampleGettingUserListenKey();
    return EXIT_SUCCESS;
}