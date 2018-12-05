#include "../storager/storager.h"
#include "../utility/file.h"
#include "../utility/io.h"

Store(Node1, file, line) {
  return __FILE__ + std::to_string(__LINE__);
}
Store(Node2, file, func) {
  return std::string(__FILE__) + __FUNCTION__;
}
Store(Node3, line, func) {
  return std::to_string(__LINE__) + __FUNCTION__;
}
int main() {
  lmss::Storager storager;
  storager.ScanSourceCode("../test");
  for (auto &node:storager.GetNodeList()) {
    srlib::println(node);
  }
  return 0;
}