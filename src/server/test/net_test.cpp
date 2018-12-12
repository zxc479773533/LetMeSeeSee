#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../storager/storager.h"
#include "../utility/file.h"
#include "../net/listener.h"
#include "../net/address.h"
#include "../net/connection.h"
#include "../net/request.h"

using namespace srlib::net;

int main(int argc, char **argv) {
  TcpConnection conn("127.0.0.1", 11111);
  auto req = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/nodelist"));
  std::cout << req.Serialize() << std::endl;
  /*for (int i = 0; i < atoi(argv[1]); i++) {
    SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test1"));
    SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test2"));
  }*/
  auto req1 = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test1"));
  std::cout << req1.Serialize() << std::endl;
  auto req2 = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test2"));
  std::cout << req2.Serialize() << std::endl;
}