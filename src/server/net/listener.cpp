#include "listener.h"
#include "address.h"
#include "connection.h"

namespace srlib {
  namespace net {
    Listener::Listener(const Address &addr, bool listen, int backlog) : File(::socket(addr.Family(), SOCK_STREAM, 0)),
      _addr(new Address(addr)) {
      if (fd() < 0) {
        perror("socket");
        return;
      }
      if (::bind(fd(), _addr->Sockaddr(), sizeof(sockaddr)) != 0) {
        perror("bind");
        return;
      }
      if (listen) {
        if (this->Listen(backlog) != 0) {
          perror("listen");
          return;
        }
      }
    }
    Listener::~Listener() {
      delete _addr;
      Close();
    }
    Listener::Listener(const String &ip, std::uint16_t port, bool listen, int backlog) : Listener(Address(ip, port),
                                                                                                  listen,
                                                                                                  backlog) {}
    Listener::Listener(const String &ip, const String &port, bool listen, int backlog) : Listener(Address(ip, port),
                                                                                                  listen,
                                                                                                  backlog) {}
    Listener::Listener(Listener &&rhs) noexcept {
      _addr = rhs._addr;
      rhs._addr = nullptr;
      SetFd(rhs.fd());
      rhs.SetFd(-1);
    }
    Listener &Listener::operator=(srlib::net::Listener &&rhs)noexcept {
      _addr = rhs._addr;
      rhs._addr = nullptr;
      SetFd(rhs.fd());
      rhs.SetFd(-1);
      return *this;
    }
    std::shared_ptr<Connection> Listener::Accept() const {
      sockaddr addr{};
      socklen_t len{sizeof(sockaddr)};
      int conn = ::accept(fd(), &addr, &len);
      if (conn < 0) {
        perror("accept");
        return nullptr;
      }
      return std::make_shared<Connection>(Address(addr), conn, false);
    }
    int Listener::Listen(int backlog) const {return ::listen(fd(), backlog);}
    const Address &Listener::GetAddress() const {return *_addr;}
    std::shared_ptr<Listener> Listen(const srlib::net::Address &addr) {
      return std::make_shared<Listener>(addr);
    }
  }
}