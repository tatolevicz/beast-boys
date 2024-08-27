//
// Created by Arthur Motelevicz on 05/03/23.
//

#include <iostream>
#include <beastboys>

int main()
{
    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    auto stream = streamer->openStream("stream.binance.com","9443","/ws/btcusdt@kline_1s", true,
    [](bool success, const std::string& data, auto stream){
        if(!success)
        {
          lg(mgutils::Info) << "Stream1 closed with msg: " << data << "\n\n";
            return;
        }

        //Work with your streamed data here
      lg(mgutils::Info) << data;
    });


    bool sent = false;
    auto messenger  = std::make_unique<bb::Messenger>();

    while(stream.lock())
    {
        // Do other stuff while the data is coming in callback
        std::this_thread::sleep_for(std::chrono::seconds(3));
//        lg(mgutils::Info) << "\nWorking.. \n";


        if(!sent)
        {
            messenger->sendMessage(stream.lock(),
             "{\n\"method\": \"SUBSCRIBE\",\n\"params\":\n[\n\"btcusdt@trade\"\n],\n\"id\": 1\n}",
             [](bool success)
             {
               if (success)
                   lg(mgutils::Info) << "Msg enviada com sucesso!";
                 else
                 lg(mgutils::Error) << "Msg nao enviada!\n";
             });

            sent = true;
        }

    }

    return 0;
}



