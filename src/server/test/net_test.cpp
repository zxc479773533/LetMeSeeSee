#include "../net/address.h"
#include "../net/connection.h"
#include "../net/listener.h"
#include "../net/request.h"
#include "../utility/file.h"
#include "../storager/storager.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

using namespace srlib::net;

void get_data(std::string req) {
  std::string content;
  std::stringstream data(req);
  getline(data, content);
  getline(data, content);
  getline(data, content, '\0');
  std::cout << content << std::endl;
}

// argc == 1: Run 1 test
// argc == 2: Run argv[1] tests
// Using time command to calculate time
int main(int argc, char **argv) {
  TcpConnection conn("127.0.0.1", 11111);
  auto req = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/nodelist"));
  // get_data(req.Serialize().std_string());
  if (argc != 1) {
    for (int i = 0; i < atoi(argv[1]); i++) {
      auto req1 = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test1"));
      auto req2 = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test2"));
      // get_data(req1.Serialize().std_string());
      // get_data(req2.Serialize().std_string());
    }
  }
  auto req1 = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test1"));
  // get_data(req1.Serialize().std_string());
  auto req2 = SendHTTPRequest(conn, HTTPRequest{}.AutoFill().Page("/Save_test2"));
  // get_data(req2.Serialize().std_string());
}