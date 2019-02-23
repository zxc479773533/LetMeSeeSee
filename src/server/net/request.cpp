#include "../utility/io.h"
#include <sys/ioctl.h>
#include "request.h"
#include "connection.h"
#include "address.h"

namespace srlib {
  namespace net {
    String HTTPRequest::Serialize() const {
      if (method.empty() || page.empty() || version.empty())return {};
      auto res = method + ' ' + page + " HTTP/" + version + "\r\n";
      for (auto &field : header) {
        if (field.second.empty())continue;
        res += field.first + ": " + field.second + "\r\n";
      }
      res += "\r\n";
      if (header.find("Content-Length") != header.end() || header.find("Transfer-Encoding") != header.end()) {
        res += content;
        res += "\r\n";
      }
      return res;
    }
    HTTPRequest HTTPRequest::Unserialize(const srlib::String &req) {
      HTTPRequest res;
      res.Version("").Page("").Method("");
      if (req.empty())return res;
      auto header_end = req.find("\r\n\r\n");
      auto header = req(0, header_end).split("\r\n");
      res.content = req(header_end + 4, req.size());
      res.method = header[0](0, header[0].find(' '));
      res.page = header[0](header[0].find('/'), header[0].rfind("HTTP/") - 1);
      res.version = header[0](header[0].rfind("HTTP/") + 5, header[0].size());
      for (int i = 1; i < header.size(); ++i) {
        auto sp = header[i].find(": ");
        res.header[header[i](0, sp)] = header[i](sp + 2, header[i].size());
      }
      return res;
    }
    String HTTPResponse::Serialize() const {
      if (version.empty() || status_code.empty() || reason_phrase.empty())return {};
      auto res = "HTTP/" + version + ' ' + status_code + ' ' + reason_phrase + "\r\n";
      for (auto &field : header) {
        if (field.second.empty())continue;
        res += field.first + ": " + field.second + "\r\n";
      }
      res += "\r\n";
      if (header.find("Content-Length") != header.end() || header.find("Transfer-Encoding") != header.end()) {
        res += content;
        res += "\r\n";
      }
      return res;
    }
    HTTPResponse HTTPResponse::Unserialize(const srlib::String &req) {
      HTTPResponse res;
      res.StatusCode("").Version("").ReasonPhrase("");
      if (req.empty())return res;
      auto header_end = req.find("\r\n\r\n");
      auto header = req(0, header_end).split("\r\n");
      res.content = req(header_end + 4, req.size());
      auto status_start = header[0].find(' ') + 1;
      res.version = header[0](header[0].find("HTTP/") + 5, status_start - 1);
      auto reason_start = header[0].find(' ', status_start) + 1;
      res.status_code = header[0](status_start, reason_start - 1);
      res.reason_phrase = header[0](reason_start, header[0].size());
      for (int i = 1; i < header.size(); ++i) {
        auto sp = header[i].find(": ");
        res.header[header[i](0, sp)] = header[i](sp + 2, header[i].size());
      }
      return res;
    }
    std::pair<String, String> splitUrl(const String &url) {
      auto partition = url.find('/');
      if (partition == std::string::npos) {
        return std::make_pair(url, "/");
      }
      return std::make_pair(url(0, partition), url(partition, url.size()));
    };
    net::HTTPResponse SendHTTPRequest(Connection &conn, const HTTPRequest &req) {
      auto s = req.Serialize();
      Slice<char> sl(const_cast<char *>(s.data()), s.size());
      for (long wr = 0; wr < s.size();) {
        auto sd = conn.Write(sl(wr));
        if (sd <= 0)break;
        wr += sd;
      }
      Array<char> buf(8192);
      HTTPResponse rep;
      for (long off = 0;;) {
        auto rd = conn.Read(buf(off));
        // Handle Connection Close/Error
        if (rd <= 0) {
          rep = HTTPResponse::Unserialize(buf(0, off).ToString());
          break;
        }
        // Parse Header
        off += rd;
        auto header_len = buf(0, off).Find("\r\n\r\n");
        // - If header is incomplete, continue to read
        if (header_len == size_t(-1))continue;
        header_len += 4;
        // - Header Read Finish, Parse Header
        rep = HTTPResponse::Unserialize(buf(0, header_len).ToString());
        // Handle Content-Length
        if (rep.header.find("Content-Length") != rep.header.end()) {
          auto continue_read = rep.header["Content-Length"].to_integer();
          // Save data to response.content
          rep.content.reserve(continue_read);
          rep.content = buf(header_len, off).ToString();
          // - Read Finish
          if (continue_read <= rep.content.size()) break;
          // - Continue to read
          for (continue_read -= rep.content.size(); continue_read != 0;) {
            rd = conn.Read(buf(0));
            if (rd <= 0) {
              break;
            } else {
              continue_read -= rd;
              rep.content += buf(0, rd).ToString();
            }
          }
          break;
        }
          // Handle Transfer-Encoding = Chunked
        else if (rep.header["Transfer-Encoding"] == "chunked"_s) {
          auto rd_off = header_len;
          while (true) {
            if (off > rd_off) {
              // Read Chunk Size
              auto size_end = buf(rd_off).Find("\r\n");
              if (size_end == 0) {
                rd_off += 2;
                continue;
              } else if (size_end == size_t(-1)) {
                break;
              }
              auto chunk_size = std::stoul(buf(rd_off, rd_off + size_end).ToString().const_std_string(), nullptr, 16);
              if (chunk_size == 0)break;
              // Move Read_Offset , +2 mean "\r\n"
              rd_off += size_end + 2;
              // Read chunk until end
              while (chunk_size > off - rd_off) {
                auto readable = off - rd_off;
                rep.content += buf(rd_off, rd_off + readable).ToString();
                chunk_size -= readable;
                rd_off = 0, off = 0;
                rd = conn.Read(buf);
                if (rd <= 0)break;
                else off += rd;
              }
              // Chunk end with "\r\n", so move Read_Offset +2
              if (chunk_size <= off - rd_off) {
                rep.content += buf(rd_off, rd_off + chunk_size).ToString();
                rd_off += chunk_size + 2;
              }
            } else {
              // Now Read_Offset == Offset, it mean buffer has been read overall. so read the fd again.
              rd_off = 0, off = 0;
              rd = conn.Read(buf);
              if (rd <= 0)break;
              else off += rd;
            }
          }
          break;
        }
      }
      return rep;
    }
    net::HTTPRequest RecvHTTPRequest(Connection &conn) {
      Array<char> buf(8192);
      HTTPRequest req;
      String str;
      size_t rd_off = 0, wr_off = 0;
      while (true) {
        auto count = conn.Read(buf(wr_off));
        if (count <= 0) {
          req.Version("");
          break;
        }
        wr_off += count;
        auto header_end = buf.Find("\r\n\r\n");
        if (header_end == size_t(-1))continue;
        header_end += 4;
        req = HTTPRequest::Unserialize(buf(0, header_end).ToString());
        if (req.header.find("Content-Length") != req.header.end()) {
          auto continue_read = req.header["Content-Length"].to_integer();
          // Save data to response.content
          req.content.reserve(continue_read);
          req.content = buf(header_end, wr_off).ToString();
          // - Read Finish
          if (continue_read == req.content.size()) break;
          // - Continue to read
          for (continue_read -= req.content.size(); continue_read != 0;) {
            count = conn.Read(buf(0));
            if (count <= 0) {
              break;
            } else {
              continue_read -= count;
              req.content += buf(0, count).ToString();
            }
          }
          break;
        } else if (req.header.find("Transfer-Encoding") != req.header.end() &&
                   req.header["Transfer-Encoding"] == "chunked"_s) {
          rd_off = header_end;
          while (true) {
            if (wr_off > rd_off) {
              // Read Chunk Size
              auto size_end = buf(rd_off).Find("\r\n");
              if (size_end == 0) {
                rd_off += 2;
                continue;
              } else if (size_end == size_t(-1)) {
                break;
              }
              auto chunk_size = std::stoul(buf(rd_off, rd_off + size_end).ToString().const_std_string(), nullptr, 16);
              if (chunk_size == 0)break;
              // Move Read_Offset , +2 mean "\r\n"
              rd_off += size_end + 2;
              // Read chunk until end
              while (chunk_size > wr_off - rd_off) {
                auto readable = wr_off - rd_off;
                req.content += buf(rd_off, rd_off + readable).ToString();
                chunk_size -= readable;
                rd_off = 0, wr_off = 0;
                count = conn.Read(buf);
                if (count <= 0)break;
                else wr_off += count;
              }
              // Chunk end with "\r\n", so move Read_Offset +2
              if (chunk_size <= wr_off - rd_off) {
                req.content += buf(rd_off, rd_off + chunk_size).ToString();
                rd_off += chunk_size + 2;
              }
            } else {
              // Now Read_Offset == Offset, it mean buffer has been read overall. so read the fd again.
              rd_off = 0, wr_off = 0;
              count = conn.Read(buf);
              if (count <= 0)break;
              else wr_off += count;
            }
          }
          break;
        } else {
          break;
        }
      }
      return req;
    }
    bool SendHTTPResponse(Connection &conn, const HTTPResponse &rep) {
      auto str = rep.Serialize();
      Slice<char> buf(const_cast<char *>(str.data()), str.size());
      int c = 0;
      while (c < str.size()) {
        auto w = conn.Write(buf(c));
        if (w <= 0)return false;
        c += w;
      }
      return true;
    }
  }
}