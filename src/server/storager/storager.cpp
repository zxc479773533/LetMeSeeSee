#include <thread>
#include "storager.h"
#include "../utility/file.h"
#include "../utility/io.h"
#include "../net/listener.h"
#include "../net/address.h"
#include "../net/connection.h"
#include "../net/request.h"

namespace lmss {
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
  void Storager::ListenAndServe(const std::string &ip, uint16_t port) {
    using namespace srlib;
    std::thread([ip, port, this]() {
      srlib::String json;
      for (auto &j : _node_list) {
        json += j + "\n";
      }
      net::Listener listener(net::Address(ip, port));
      while (true) {
        auto conn = listener.Accept();
        println("Connection from", conn->GetAddress().Ip() + ":" + std::to_string(conn->GetAddress().Port()));
        std::thread([](std::shared_ptr<net::Connection> conn, const String &json) {
          while (true) {
            auto req = net::RecvHTTPRequest(*conn);
            // println(req.Serialize());
            if (req.version.empty())return;
            // Request NodeList
            if (req.page == "/nodelist") {
              net::SendHTTPResponse(*conn,
                                    net::HTTPResponse{}.AutoFill()
                                                       .Header("Content-Length", std::to_string(json.size()))
                                                       .Content(json));
            } else {
              // Request Node
              auto node_name = req.page(req.page.find('/') + 1, req.page.size());
              auto file_name = Storager::Call(node_name.std_string());
              if (!file_name.empty()) {
                auto file = OpenFile(file_name);
                net::SendHTTPResponse(*conn,
                                      net::HTTPResponse{}.AutoFill()
                                                         .Header("Content-Length", std::to_string(file.Size()))
                                                         .Content(file.ReadAll()));
              } else {
                net::SendHTTPResponse(*conn,
                                      net::HTTPResponse{}.Version("1.1")
                                                         .StatusCode("404")
                                                         .Header("Content-Length", std::to_string(13))
                                                         .ReasonPhrase("Not Found")
                                                         .Content("404 not found"));
              }
            }
          }
        }, conn, json).detach();
      }
    }).detach();
  }
}