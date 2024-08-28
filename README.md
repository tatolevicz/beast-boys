
# Beastboys Library

Beastboys is a C++ library designed for handling network communication, including raw socket, WebSocket, and REST API connections. It provides a robust framework for building networked applications with support for both synchronous and asynchronous operations.

## Features

- **Raw Socket Connections**: Manage low-level socket connections with full control over data transmission.
- **WebSocket Connections**: Establish WebSocket connections for real-time communication with support for SSL.
- **REST API**: Interact with RESTful services using HTTP/HTTPS, supporting various HTTP methods and response handling.

## Installation

To use the Beastboys library in your project, you need to include it in your CMake configuration. Add the following to your `CMakeLists.txt`:

```cmake
find_package(beastboys REQUIRED)
target_link_libraries(your_project PRIVATE beastboys)
```

Ensure you have all dependencies installed, including Boost, Catch2 for testing, and any SSL libraries if you're planning to use WebSocket SSL.

## Usage

### Raw Socket Connections

To establish a raw socket connection:

```cpp
#include <beastboys>
#include <thread>

int main() {
    bb::network::server::Server server;
    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    std::thread serverThread([&]() {
        server.start<bb::network::server::SocketDoorman>(1234);
    });

    // Give the server some time to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Connect to the server
    auto stream = streamer->openStream("localhost", "1234", "", false, nullptr);
    auto streamPtr = stream.lock();
    if (streamPtr && streamPtr->isOpen()) {
        std::cout << "Connected successfully to raw socket server." << std::endl;
    } else {
        std::cerr << "Failed to connect to raw socket server." << std::endl;
    }

    serverThread.join();
    return 0;
}
```

### WebSocket Connections

To create a WebSocket connection:

```cpp
#include <beastboys>
#include <thread>

int main() {
    bb::network::server::Server server;
    std::shared_ptr<bb::Streamer> streamer(new bb::Streamer());

    std::thread serverThread([&]() {
        server.start<bb::network::server::ws::WebSocketDoorman>(1234);
    });

    // Give the server some time to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Connect to WebSocket server
    auto stream = streamer->openStream("localhost", "1234", "", false, nullptr);
    auto streamPtr = stream.lock();
    if (streamPtr && streamPtr->isOpen()) {
        std::cout << "Connected successfully to WebSocket server." << std::endl;
    } else {
        std::cerr << "Failed to connect to WebSocket server." << std::endl;
    }

    serverThread.join();
    return 0;
}
```

### REST API

To perform HTTP requests using the REST API component:

```cpp
#include <beastboys>

int main() {
    auto api = std::make_shared<bb::network::rest::RestApi>("80", bb::network::rest::TaskExecutionType::BB_SYNCH, 10000);
    auto settings = bb::network::rest::NetworkRequestSettings();

    settings.setFullUrl("http://jsonplaceholder.typicode.com/posts/1")
            .setContentType(bb::network::rest::ContentType::JSON);

    api->get(settings, [&](const bb::network::rest::NetworkResponse &response) {
        if (response.isOk()) {
            std::cout << "GET request successful, response code: " << response.http_result_code << std::endl;
        } else {
            std::cerr << "GET request failed." << std::endl;
        }
    });

    return 0;
}
```

## Running Tests

The Beastboys library comes with a comprehensive suite of tests. To run the tests, ensure you have Catch2 installed and configured in your project. Use the following command to execute all tests:

```bash
ctest
```

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

## License

Beastboys is licensed under the MIT License. See `LICENSE` for more information.

## Authors

* **Tato Levicz** - [@tatolevicz](https://github.com/tatolevicz)
