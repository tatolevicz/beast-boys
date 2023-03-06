//
// Created by Arthur Motelevicz on 05/03/23.
//

#include <iostream>
#include <beastboys>

int main(){
    std::shared_ptr<bb::network::ws::Websocket> impl(new bb::network::ws::Websocket());

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



