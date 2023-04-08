//
// Created by Arthur Motelevicz on 05/03/23.
//

#include <iostream>
#include <beastboys>

void localHostStream();

std::shared_ptr< bb::network::ws::Stream> createStream(const std::shared_ptr<bb::Streamer> & streamer){

    auto stream = streamer->openStream("localhost","1234","", false, [](bool success, const std::string& data, auto stream){
        if(!success) {
            std::cout << "Stream1 closed with msg: " << data << "\n\n";
            //here in the client you can reschedule a reconnection routine
            return;
        }

        //Work with your streamed data here
        std::cout << data << "\n\n";
    });

    return std::move(stream.lock());

}
void localHostStream(){
    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    auto stream = createStream(streamer);
    std::function<void(SharedStream)> closeCB = [&](SharedStream closedStream){
        std::cout << "Stream CLOSE CB!!! \n";
        stream = createStream(streamer);
        stream->setCloseStreamCallback(closeCB);
    };

    stream->setCloseStreamCallback(closeCB);

    stream->setCloseStreamCallback([&](SharedStream closedStream){
        closeCB(closedStream);
    });

    while(stream.use_count() > 1){
        // Do other stuff while the data is coming in callback
//        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

}

int main(){

    localHostStream();

    return 0;

    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    auto stream = streamer->openStream("stream.binance.com","9443","/ws/btcusdt@kline_1s", true, [](bool success, const std::string& data, auto stream){
        if(!success) {
            std::cout << "Stream1 closed with msg: " << data << "\n\n";
            return;
        }

        //Work with your streamed data here
        std::cout << data << "\n\n";
    });


    bool sent = false;
    auto messenger  = std::make_unique<bb::Messenger>();

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



