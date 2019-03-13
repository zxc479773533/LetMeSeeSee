#include <unistd.h>
#include <iostream>
#include "../storager/storager.h"
#include "../utility/file.h"
#include "../utility/io.h"

int test1;
std::string test2;

Store(Save_test1, test1) {
  auto f = fopen("test1.txt", "w+");
  std::string test1_str =  std::to_string(test1);
  fwrite(test1_str.c_str(), 1, test1_str.size(), f);
  fclose(f);
  return "test1.txt";
}

Store(Save_test2, test2) {
  auto f = fopen("test2.txt", "w+");
  fwrite(test2.c_str(), 1, test2.size(), f);
  fclose(f);
  return "test2.txt";
}

// argc == 1: Run scan test
// argc == 2: Start network service
// Using time command to calculate time
int main(int argc, char **argv) {
  lmss::Storager storager;
  storager.SetLogFile("test.log");
  if (argc != 1) {
    storager.ScanSourceCode(argv[1]);
    auto nodelist_file = srlib::OpenFile("scan_test.json");
    for (auto &node : storager.GetNodeList()) {
      nodelist_file.Write(node);
    }
    exit(1);
  }
  storager.ScanSourceCode("../test");
  auto nodelist_file = srlib::OpenFile("node_list.json");
  for (auto &node : storager.GetNodeList()) {
    nodelist_file.Write(node);
  }
  storager.SetPassword("zxc123!@#");
  storager.ListenAndServe("0.0.0.0", 11111);
  test1 = 0;
  char ch = 'A';
  while (true) {
    test1 += 1;
    test1 %= 100;
    test2 += ch;
    if (ch == 'Z' + 1) {
      ch = 'A';
      test2 = "";
    } else
      ch += 1;
    usleep(500000);
  }
}