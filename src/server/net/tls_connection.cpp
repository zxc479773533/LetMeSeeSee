#include "tls_connection.h"
#include "address.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace srlib {
  namespace net {
    void SSLInit() {
      OpenSSL_add_all_algorithms();
      SSL_library_init();
      SSL_load_error_strings();
      printf("SSL Initialized!\n");
    }
    TlsConnection::TlsConnection(const Address &addr, bool connect) : TcpConnection(addr, connect) {
      _method = TLSv1_2_client_method();
      _ctx = SSL_CTX_new(_method);
      _ssl = SSL_new(_ctx);
      SSL_set_fd(_ssl, fd());
      if (SSL_connect(_ssl) < 0) {
        ERR_print_errors_fp(stdout);
      }
    }
    TlsConnection::~TlsConnection() {
      SSL_free(_ssl);
      SSL_CTX_free(_ctx);
      Close();
    }
    TlsConnection::TlsConnection(srlib::net::TlsConnection &&rhs)noexcept {
      _ssl = rhs._ssl;
      _ctx = rhs._ctx;
      _method = rhs._method;
      rhs._ssl = nullptr;
      rhs._ctx = nullptr;
      rhs._method = nullptr;
    }
    TlsConnection &TlsConnection::operator=(srlib::net::TlsConnection &&rhs)noexcept {
      _ssl = rhs._ssl;
      _ctx = rhs._ctx;
      _method = rhs._method;
      rhs._ssl = nullptr;
      rhs._ctx = nullptr;
      rhs._method = nullptr;
      return *this;
    }
    TlsConnection::TlsConnection(const String &ip, uint16_t port, bool connect) : TlsConnection(Address(ip, port),
                                                                                                connect) {}
    TlsConnection::TlsConnection(const String &ip, const String &port, bool connect) : TlsConnection(Address(ip, port),
                                                                                                     connect) {}
    String TlsConnection::Read(size_t size) const {
      auto buf = new char[size]{};
      auto count = SSL_read(_ssl, buf, size);
      if (count < 0) {
        ERR_print_errors_fp(stderr);
        delete buf;
        return String{};
      }
      auto res = String(buf, count);
      delete[]buf;
      return res;
    }
    ssize_t TlsConnection::Read(srlib::Array<char> &buf) {
      return ::SSL_read(_ssl, buf.Data(), buf.Size());
    }
    ssize_t TlsConnection::Read(srlib::Array<uint8_t> &buf) {
      return ::SSL_read(_ssl, buf.Data(), buf.Size());
    }
    ssize_t TlsConnection::Read(srlib::Slice<char> buf) {
      return ::SSL_read(_ssl, buf.Data(), buf.Size());
    }
    ssize_t TlsConnection::Read(srlib::Slice<uint8_t> buf) {
      return ::SSL_read(_ssl, buf.Data(), buf.Size());
    }
    ssize_t TlsConnection::Write(const srlib::String &msg) const {
      return SSL_write(_ssl, msg.data(), msg.size());
    }
    ssize_t TlsConnection::Write(const srlib::Array<char> &buf) {
      return ::SSL_write(_ssl, buf.Data(), buf.Size());
    }
    ssize_t TlsConnection::Write(const srlib::Array<uint8_t> &buf) {
      return ::SSL_write(_ssl, buf.Data(), buf.Size());
    }
    ssize_t TlsConnection::Write(const srlib::Slice<char> &buf) {
      return ::SSL_write(_ssl, buf.Data(), buf.Size());
    }
    ssize_t TlsConnection::Write(const srlib::Slice<uint8_t> &buf) {
      return ::SSL_write(_ssl, buf.Data(), buf.Size());
    }
    int TlsConnection::Pending() {
      return SSL_pending(_ssl);
    }
  }
}