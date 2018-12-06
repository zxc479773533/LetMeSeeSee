#include <sys/socket.h>
#include "connection.h"
#include "address.h"

namespace srlib {
  namespace net {
    Connection::Connection(const Address &addr, int fd, bool connect) : File(fd), _connected(false) {
      if (fd < 0) {
        fprintf(stderr, "Connection Construct Error: fd < 0.\n");
        _addr = nullptr;
        return;
      }
      _addr = new Address(addr);
      if (connect) {
        if (Connect() == 0)_connected = true;
        else {
          perror("connect");
          _connected = false;
        }
      } else {
        _connected = false;
      }
    }
    Connection::~Connection() {
      delete _addr;
      Close();
    }
    Connection::Connection(const srlib::String &ip, std::uint16_t port, int fd, bool connect) : Connection(Address(ip,
                                                                                                                   port),
                                                                                                           fd) {}
    Connection::Connection(const srlib::String &ip, const srlib::String &port, int fd, bool connect) : Connection(
      Address(ip, port),
      fd) {}
    Connection::Connection(srlib::net::Connection &&old)noexcept {
      _addr = old._addr;
      _connected = old._connected;
      old._addr = nullptr;
    }
    Connection &Connection::operator=(srlib::net::Connection &&old)noexcept {
      _addr = old._addr;
      _connected = old._connected;
      old._addr = nullptr;
      return *this;
    }
    bool Connection::IsConnected() {return _connected;}
    int Connection::Disconnect() {
      if (_connected) {
        auto res = Close();
        if (res == 0)_connected = false;
      }
    }
    ssize_t Connection::Send(const srlib::String &msg, int flag) {
      if (!_connected) {
        fprintf(stderr, "This Socket is NOT Connected!\n");
        return -1;
      }
      return ::send(fd(), msg.c_str(), msg.size(), flag);
    }
    String Connection::Recv(size_t size, int flag) {
      if (!_connected) {
        fprintf(stderr, "This Socket is NOT Connected!\n");
        return String();
      }
      auto buf = new char[size];
      auto count = ::recv(fd(), buf, size, flag);
      String res(buf, count);
      delete buf;
      return res;
    }
    void Connection::SetSendTimeout(const struct timeval &timeout) {
      if (setsockopt(fd(), SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeval)) < 0)perror("setsockopt");
    }
    void Connection::SetRecvTimeout(const struct timeval &timeout) {
      if (setsockopt(fd(), SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeval)) < 0)perror("setsockopt");
    }
    Address &Connection::GetAddress() {return *_addr;}
    TcpConnection::TcpConnection(const srlib::net::Address &addr, bool connect) : Connection(addr,
                                                                                             ::socket(addr.Family(),
                                                                                                      SOCK_STREAM,
                                                                                                      0),
                                                                                             connect) {
    }
    TcpConnection::TcpConnection(const srlib::String &ip, std::uint16_t port, bool connect) : TcpConnection(Address(ip,
                                                                                                                    port,
                                                                                                                    connect)) {}
    TcpConnection::TcpConnection(const srlib::String &ip, const srlib::String &port, bool connect) : TcpConnection(
      Address(ip, port),
      connect) {}
    int Connection::Connect() {
      if (_connected) {
        fprintf(stderr, "This Socket is Already Connected!\n");
      }
      return ::connect(fd(), GetAddress().Sockaddr(), sizeof(sockaddr));
    }
    UdpConnection::UdpConnection(const srlib::net::Address &addr, bool connect) : Connection(addr,
                                                                                             ::socket(addr.Family(),
                                                                                                      SOCK_DGRAM,
                                                                                                      0),
                                                                                             connect) {}
    UdpConnection::UdpConnection(const srlib::String &ip, std::uint16_t port, bool connect) : UdpConnection(Address(ip,
                                                                                                                    port),
                                                                                                            connect) {}
    UdpConnection::UdpConnection(const srlib::String &ip, const srlib::String &port, bool connect) : UdpConnection(
      Address(ip, port),
      connect) {}
    ssize_t UdpConnection::SendTo(const srlib::String &msg, const Address &addr, int flag) {
      return ::sendto(fd(), msg.c_str(), msg.size(), flag, addr.Sockaddr(), sizeof(sockaddr));
    }
    std::pair<String, Address> UdpConnection::RecvFrom(size_t size, int flag) {
      sockaddr addr{};
      socklen_t len;
      auto buf = new char[size];
      auto count = ::recvfrom(fd(), buf, size, flag, &addr, &len);
      String res(buf, count);
      delete buf;
      return std::make_pair(std::move(res), Address(addr));
    }
    int UdpConnection::Bind() {return ::bind(fd(), GetAddress().Sockaddr(), sizeof(sockaddr));}
  }
}