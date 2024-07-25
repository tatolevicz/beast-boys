//
// Created by Arthur Motelevicz on 24/07/24.
//
#include <boost/asio.hpp>
#include <beastboys>

int main() {
  boost::asio::io_context io_context;

  // Create a RawSocket object and start connection
  auto socket = std::make_shared<RawSocketImpl>(io_context);

  // Connect to the server (host and port should be appropriate for your telnet server)
  socket->connect("datafeed1.cedrotech.com", "81");

//  socket->send("\n");

  // Run the io_context to start asynchronous operations
  io_context.run();

  // Send a message
//  socket->send("!");

  // Run the io_context again to ensure all async operations complete
  io_context.run();

  // Close the connection after usage
  socket->close();

  return 0;
}
