#ifndef UTILITY_REQUEST_H
#define UTILITY_REQUEST_H

#include "utility/exstring.h"

namespace srlib {
  class String;
  namespace net {
    class Connection;
    
    class TlsConnection;
    
    struct HTTPRequest {
      String method;
      String page;
      String version;
      std::map<String, String> header;
      String content;
      HTTPRequest &Method(const String &m) {
        method = m;
        return *this;
      }
      HTTPRequest &Page(const String &p) {
        page = p;
        return *this;
      }
      HTTPRequest &Version(const String &v) {
        version = v;
        return *this;
      }
      HTTPRequest &Content(const String &c) {
        content = c;
        return *this;
      }
      HTTPRequest &Header(const String &key, const String &value) {
        header[key] = value;
        return *this;
      }
      HTTPRequest &AutoFill() {
        return (*this).Method("GET")
                      .Page("/")
                      .Version("1.1")
                      .Content("")
                      .Header("User-Agent",
                              "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0")
                      .Header("Connection", "keep-alive")
                      .Header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8")
                      .Header("Accept-Encoding", "deflate");
      }
      String Serialize() const;
      static HTTPRequest Unserialize(const String &req);
    };
    
    struct HTTPResponse {
      String version;
      String status_code;
      String reason_phrase;
      std::map<String, String> header;
      String content;
      String Serialize() const;
      HTTPResponse &Version(const String &v) {
        version = v;
        return *this;
      }
      HTTPResponse &StatusCode(const String &s) {
        status_code = s;
        return *this;
      }
      HTTPResponse &ReasonPhrase(const String &r) {
        reason_phrase = r;
        return *this;
      }
      HTTPResponse &Content(const String &c) {
        content = c;
        return *this;
      }
      HTTPResponse &Header(const String &key, const String &value) {
        header[key] = value;
        return *this;
      }
      HTTPResponse &AutoFill() {
        return (*this).Version("1.1").StatusCode("200").ReasonPhrase("OK").Content("");
      }
      static HTTPResponse Unserialize(const String &req);
    };
    
    String httpGet(const String &url, const String &append = {}, std::uint64_t maxSize = 1024 * 1024);
    String httpsGet(const String &url, const std::vector<String> &append = {});
    String httpsGet(TlsConnection &conn, const String &url, const std::vector<String> &append = {});
    HTTPResponse SendHTTPRequest(Connection &conn, const HTTPRequest &req);
    HTTPRequest RecvHTTPRequest(Connection &conn);
    std::pair<String, String> splitUrl(const String &url);
  }
}

#endif
