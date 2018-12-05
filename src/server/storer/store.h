#ifndef LETMESEESEE_STORE_H
#define LETMESEESEE_STORE_H

#include <unordered_map>
#include "../utility/singleton.h"
#include "../utility/io.h"
#include "../utility/file.h"

namespace sono {
  
  typedef std::string(*store_t)();
  template<typename T> using Reflector = std::unordered_map<std::string, T>;
#define private public
  
  class Storager {
  private:
    typedef srlib::Singleton<Reflector<store_t>> reflector;
  private:
    std::vector<std::string> _node_list;
  public:
    static void Register(const std::string &name, store_t func) {reflector::GetInstance()[name] = func;}
    static std::string Call(const std::string &name) {
      if (reflector::GetInstance()[name]) {
        return reflector::GetInstance()[name]();
      }
      srlib::logln("Reflector Warning: No Matching Function:", name);
      return {};
    }
    void ScanSourceCode(const std::string &path) {
      srlib::OpenDir(path).RecursiveWalk([this](srlib::FileInfo &&info) {
        for (auto &line:srlib::OpenFile(info.Path()).ReadAll().split("\n")) {
          std::string reg(R"(^ *Store\(([^)]*)\).*$)");
          if (line.regex_match(reg)) {
            auto vec = line.regex_replace(reg, "$1").split(",");
            for (auto &v:vec) {
              v = v.trim();
            }
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
  };

#define Store(name, ...) std::string name();\
static auto name##helper = (sono::Storager::Register(#name,name),0);\
std::string name()
}

#endif
