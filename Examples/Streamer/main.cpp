//
// Created by Arthur Motelevicz on 05/03/23.
//

#include <iostream>
#include "WebsocketImpl.h"
#include "Stream.h"

int main(){
    std::shared_ptr<bb::network::ws::WebsocketImpl> impl(new bb::network::ws::WebsocketImpl());

    auto stream2 = impl->openStream("localhost","1234","", false, [](bool success, const std::string& data){
        if(success)
            std::cout << data;
        else
            std::cout << "Stream closed with error: " << data << "\n\n";
    });

    while(stream2.lock()){

    }

    return 0;
}



