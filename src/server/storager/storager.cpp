#include <thread>
#include <atomic>
#include "storager.h"
#include "../utility/file.h"
#include "../utility/io.h"
#include "../net/listener.h"
#include "../net/address.h"
#include "../net/connection.h"
#include "../net/request.h"
#include "../utility/logger.h"
#include "../utility/md5.h"

using namespace srlib::log;

namespace lmss {
  std::string Storager::Call(const std::string &name) {
    auto &f = reflector::GetInstance()[name];
    if (f) {
      return f();
    }
    Log.Logln("Reflector Warning: No Matching Function: " + name);
    return {};
  }
  void Storager::SetLogFile(const std::string &filename) {
    Log.SetOutputFile(filename);
  }
  void Storager::ScanSourceCode(const std::string &path) {
    Log.Logln("Scan Source Code Begin...");
    // 遍历目录，对目录下的每一个文件进行扫描。
    srlib::OpenDir(path).RecursiveWalk([this](srlib::FileInfo &&info) {
      for (auto &line:srlib::OpenFile(info.Path()).ReadAll().split("\n")) {
        // 找到包含了Store宏的行。
        line.trim();
        if (line.has_prefix("Store(")) {
          auto vec = line(6, line.find(')')).split(",");
          for (auto &v:vec) {
            v.trim();
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
          _node_list.emplace_back(json.std_string_copy());
        }
      }
    });
    Log.Logln("Scan Source Code End. Found " + std::to_string(_node_list.size()) + " Node(s).");
  }
  void Storager::SetPassword(const std::string &password) {
    _password = srlib::MD5::Hash(password);
  }
  std::atomic<int> client_count;
  void Storager::ListenAndServe(const std::string &ip, uint16_t port) {
    using namespace srlib;
    std::thread([ip, port, this]() {
      srlib::String json = "[";
      for (auto &j : _node_list) {
        json += j + ",";
      }
      if (json.size() > 1)json[json.size() - 1] = ']';
      else json += ']';
      net::Listener listener(net::Address(ip, port));
      while (true) {
        auto conn = listener.Accept();
        if (client_count >= 20) {
          conn->Close();
          continue;
        }
        client_count++;
        std::thread([](std::shared_ptr<net::Connection> conn, const String &json, const std::string &password) {
          auto addr = conn->GetAddress().Ip() + ":" + std::to_string(conn->GetAddress().Port());
          Log.Logln("Connection from " + addr.std_string());
          while (true) {
            auto req = net::RecvHTTPRequest(*conn);
            if (req.version.empty()) {
              Log.Logln(addr.std_string() + " Receive HTTP Request Failed.");
              break;
            }
            // Password Verification
            if (req.header["Password"] != password) {
              Log.Logln(addr.std_string() + "Error Password.");
              net::SendHTTPResponse(*conn,
                                    net::HTTPResponse{}.Version("1.1")
                                                       .StatusCode("404")
                                                       .Header("Content-Length", std::to_string(13))
                                                       .ReasonPhrase("Not Found")
                                                       .Content("404 not found"));
              break;
            }
            // No Command
            if (req.header["CMD"].empty()) {
              Log.Logln(addr.std_string() + "Command Is Empty.");
              break;
            }
            // Request NodeList
            if (req.header["CMD"] == "nodelist") {
              Log.Logln(addr.std_string() + "GET nodelist");
              net::SendHTTPResponse(*conn,
                                    net::HTTPResponse{}.AutoFill()
                                                       .Header("Content-Length", std::to_string(json.size()))
                                                       .Content(json));
            } else {
              // Request Node
              auto node_name = req.header["CMD"];
              auto file_name = Storager::Call(node_name.std_string());
              if (!file_name.empty()) {
                Log.Logln(addr.std_string() + "GET " + node_name.std_string() + " Success.");
                auto file = OpenFile(file_name);
                net::SendHTTPResponse(*conn,
                                      net::HTTPResponse{}.AutoFill()
                                                         .Header("Content-Length", std::to_string(file.Size()))
                                                         .Content(file.ReadAll()));
              } else {
                Log.Logln(addr.std_string() + "GET " + node_name.std_string() + " Failed.");
                net::SendHTTPResponse(*conn,
                                      net::HTTPResponse{}.Version("1.1")
                                                         .StatusCode("404")
                                                         .Header("Content-Length", std::to_string(13))
                                                         .ReasonPhrase("Not Found")
                                                         .Content("404 not found"));
              }
            }
          }
          client_count--;
        }, conn, json, _password).detach();
      }
    }).detach();
  }
}