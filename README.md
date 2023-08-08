# BeastBoys

> Only tested on macos for now.

BeastBoys is a is a collection of helpers classes built over boost::beast, designed to make it easier to open WebSocket streams and REST requests. This library is intended to simplify the implementation of real-time communication and API interactions in modern C++ projects.

## Required packages

You will need to install [boost](http://boost.org)  and [openssl](https://www.openssl.org).

Below some instructions how to do it:

MACOS -> install it using brew:
```
brew install boost
brew install openssl@1.1
```

## Features

Simple and intuitive API

WebSocket client implementation (using SSL or not)

REST client implementation

Lightweight


## Example Usage

Here's a basic example to open a TCP socket:

```

#include <iostream>
#include <beastboys>


int main(){

    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    auto stream = streamer->openStream("stream.binance.com",
                                        "9443",
                                        "/ws/btcusdt@kline_1s",
                                        true, //uses SSL
    [](bool success, const std::string& data, auto stream){
        if(!success) {
            std::cout << "Stream1 closed with msg: " << data << "\n\n";
            return;
        }

        //Work with your streamed data here
        std::cout << data << "\n\n";

    });
    
    // Get 10 seconds of market data
    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}

```

## Authors

* **Tato Levicz** - [@tatolevicz](https://github.com/tatolevicz)


