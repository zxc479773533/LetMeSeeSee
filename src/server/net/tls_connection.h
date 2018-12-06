#ifndef UTILITY_TLS_CONNECTION_H
#define UTILITY_TLS_CONNECTION_H

#include "connection.h"

struct ssl_st;
struct ssl_method_st;
struct ssl_ctx_st;
typedef struct ssl_st SSL;
typedef struct ssl_method_st SSL_METHOD;
typedef struct ssl_ctx_st SSL_CTX;
namespace srlib {
  namespace net {
    void SSLInit();
    
    class SSLInitializer {
    public:
      SSLInitializer() {SSLInit();}
    };
    
    //static SSLInitializer& init = Singleton<SSLInitializer>::GetInstance();
    
    class TlsConnection : public TcpConnection {
    private:
      SSL *_ssl = nullptr;
      const SSL_METHOD *_method = nullptr;
      SSL_CTX *_ctx = nullptr;
    public:
      TlsConnection() = default;
      ~TlsConnection() override;
      TlsConnection(TlsConnection &&rhs)noexcept;
      TlsConnection &operator=(TlsConnection &&rhs)noexcept;
      TlsConnection(const Address &addr, bool connect = true);
      TlsConnection(const String &ip, uint16_t port, bool connect = true);
      TlsConnection(const String &ip, const String &port, bool connect = true);
      ssize_t Write(const String &msg) const override;
      String Read(size_t size) const override;
      ssize_t Read(Array<char> &buf) override;
      ssize_t Read(Array<uint8_t> &buf) override;
      ssize_t Read(Slice<char> buf) override;
      ssize_t Read(Slice<uint8_t> buf) override;
      ssize_t Write(const Array<char> &buf) override;
      ssize_t Write(const Array<uint8_t> &buf) override;
      ssize_t Write(const Slice<char> &buf) override;
      ssize_t Write(const Slice<uint8_t> &buf) override;
      int Pending() override;
    };
  }
}

#endif
