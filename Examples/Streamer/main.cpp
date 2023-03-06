//
// Created by Arthur Motelevicz on 05/03/23.
//

#include <iostream>
#include <beastboys>

int main(){
    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    auto stream = streamer->openStream("localhost","1234","", false, [](bool success, const std::string& data){
        if(success)
            std::cout << data;
        else
            std::cout << "Stream closed with error: " << data << "\n\n";
    });

    while(stream.lock()){

    }

    return 0;
}



