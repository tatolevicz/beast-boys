
# Beastboys Library

Beastboys is a C++ library designed for handling network communication, including raw socket, WebSocket, and REST API connections. It provides a robust framework for building networked applications with support for both synchronous and asynchronous operations.

## Features

- **Raw Socket Connections**: Manage low-level socket connections with full control over data transmission.
- **WebSocket Connections**: Establish WebSocket connections for real-time communication with support for SSL.
- **REST API**: Interact with RESTful services using HTTP/HTTPS, supporting various HTTP methods and response handling.

### Required Packages

> **⚠️ Warning:** This project has only been tested on macOS.

To use `beastboys`, you need to install the following packages:

- [Boost (minimum version 1.83.0)](http://boost.org)
- [OpenSSL(version 1.1)](https://www.openssl.org)

Additionally, `beastboys` depends on the `mgutils` library as a submodule. The Boost library is also a dependency of `mgutils`.


#### macOS Installation

Install the required packages using Homebrew:

```sh
brew install boost@1.83.0
brew install openssl@1.1
```

#### Linux Installation

On Linux, you can install Boost and OpenSSL using your package manager. For example, on Ubuntu:

```sh
sudo apt-get update
sudo apt-get install -y libboost1.83-dev libssl1.1-dev
```

If Boost 1.83.0 is not available in your distribution, you may need to download and build it manually from the [Boost website](https://www.boost.org/users/download/).

#### Windows Installation

On Windows, you can install Boost and OpenSSL using a package manager like vcpkg:

```sh
vcpkg install boost@1.83.0
vcpkg install openssl@1.1
```

Make sure to integrate vcpkg with your Visual Studio environment:

```sh
vcpkg integrate install
```


## Documentation and Tests
The test files included in this repository serve as living documentation. They provide concrete examples of how to use the various features of the beastboys library, including managing raw socket connections, WebSocket communication, and interacting with REST APIs. By examining and running these tests, users can gain a better understanding of the library's functionality and intended use cases.



## Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

## License

Beastboys is licensed under the MIT License. See `LICENSE` for more information.

## Authors

* **Tato Levicz** - [@tatolevicz](https://github.com/tatolevicz)
