#ifndef UTILITY_CONNECTION_H
#define UTILITY_CONNECTION_H

#include "utility/file.h"

namespace srlib {
  class String;
  namespace net {
    class Address;
    
    class Connection : public File {
    private:
      Address *_addr;
      bool _connected;
    public:
      Connection() = default;
      ~Connection() override;
      Connection(const Address &addr, int fd, bool connect = true);
      Connection(const String &ip, const String &port, int fd, bool connect = true);
      Connection(const String &ip, std::uint16_t port, int fd, bool connect = true);
      Connection(const Connection &rhs) = delete;
      Connection &operator=(const Connection &rhs) = delete;
      Connection(Connection &&old)noexcept;
      Connection &operator=(Connection &&old)noexcept;
      int Connect();
      bool IsConnected();
      int Disconnect();
      ssize_t Send(const String &msg, int flag = 0);
      String Recv(size_t size, int flag = 0);
      void SetSendTimeout(const struct timeval &timeout);
      void SetRecvTimeout(const struct timeval &timeout);
      Address &GetAddress();
    };
    
    class TcpConnection : public Connection {
    private:
    public:
      TcpConnection() = default;
      TcpConnection(const Address &addr, bool connect = true);
      TcpConnection(const String &ip, const String &port, bool connect = true);
      TcpConnection(const String &ip, std::uint16_t port, bool connect = true);
    };
    
    class UdpConnection : public Connection {
    private:
    public:
      UdpConnection() = default;
      UdpConnection(const Address &addr, bool connect = false);
      UdpConnection(const String &ip, const String &port, bool connect = false);
      UdpConnection(const String &ip, std::uint16_t port, bool connect = false);
      int Bind();
      ssize_t SendTo(const String &msg, const Address &addr, int flag = 0);
      std::pair<String, Address> RecvFrom(size_t size, int flag = 0);
    };
  }
}

#endif
