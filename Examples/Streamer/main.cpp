//
// Created by Arthur Motelevicz on 05/03/23.
//

#include <iostream>
#include <beastboys>

int main(){
    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    auto stream = streamer->openStream("localhost","1234","", false, [](bool success, const std::string& data){
        if(success) {
            //Work with your streamed data here
            std::cout << data;
        }
        else
            std::cout << "Stream closed with msg: " << data << "\n\n";
    });

    while(stream.lock()){
        // Do other stuff while the data is coming in callback
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\nWorking.. \n";
    }

    return 0;
}



