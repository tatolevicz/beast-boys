//
// Created by Arthur Motelevicz on 05/03/23.
//

#include <iostream>
#include <beastboys>

int main(){
    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    auto stream = streamer->openStream("stream.binance.com","9443","/ws/btcusdt@kline_1s", true, [](bool success, const std::string& data){
        if(!success) {
            std::cout << "Stream1 closed with msg: " << data << "\n\n";
            return;
        }

        //Work with your streamed data here
        std::cout << data << "\n\n";
    });

    bool sent = false;
    std::shared_ptr<bb::Messenger> messenger  = std::make_unique<bb::Messenger>();

    while(stream.lock()){
        // Do other stuff while the data is coming in callback
        std::this_thread::sleep_for(std::chrono::seconds(3));
//        std::cout << "\nWorking.. \n";

        if(!sent)
        {
            messenger->sendMassage(stream.lock(),
                                   "{\n\"method\": \"SUBSCRIBE\",\n\"params\":\n[\n\"btcusdt@trade\"\n],\n\"id\": 1\n}",
            [](bool success){
                if(success)
                    std::cout << "Msg enviada com sucesso!\n";
                else
                    std::cerr << "Msg nao enviada!\n";
            });

            sent = true;
        }

    }

    return 0;
}



