#include "store.h"
#include "../utility/file.h"
#include "../utility/io.h"

namespace sono {
  std::string Storager::Call(const std::string &name) {
    auto &f = reflector::GetInstance()[name];
    if (f) {
      return f();
    }
    srlib::logln("Reflector Warning: No Matching Function:", name);
    return {};
  }
  void Storager::ScanSourceCode(const std::string &path) {
    // 遍历目录，对目录下的每一个文件进行扫描。
    srlib::OpenDir(path).RecursiveWalk([this](srlib::FileInfo &&info) {
      std::string reg(R"(^ *Store\(([^)]*)\).*$)");
      for (auto &line:srlib::OpenFile(info.Path()).ReadAll().split("\n")) {
        // 找到包含了Store宏的行。
        if (line.regex_match(reg)) {
          auto vec = line.regex_replace(reg, "$1").split(",");
          for (auto &v:vec) {
            v = v.trim();
          }
          // json化
          auto json = srlib::String(R"({"node": ")") + vec[0] + "\", " + R"("data": [)";
          for (int i = 1; i < vec.size(); ++i) {
            if (i == vec.size() - 1) {
              json += '"' + vec[i] + "\"";
            } else {
              json += '"' + vec[i] + "\", ";
            }
          }
          json += "]}";
          _node_list.emplace_back(std::move(json.std_string_copy()));
        }
      }
    });
  }
}