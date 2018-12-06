#include "connector.h"
#include "connection.h"
#include "address.h"

namespace srlib {
  namespace net {
    std::shared_ptr<Connection> Dial(const srlib::net::Address &addr) {
      return std::make_shared<TcpConnection>(addr);
    }
    Connector::Connector(const srlib::net::Address &addr) : _addr(new Address(addr)) {}
    Connector::~Connector() {delete _addr;}
    Connector::Connector(const srlib::net::Connector &rhs) {
      _addr = new Address(*rhs._addr);
    }
    Connector &Connector::operator=(const srlib::net::Connector &rhs) {
      _addr = new Address(*rhs._addr);
      return *this;
    }
    Connector::Connector(srlib::net::Connector &&rhs)noexcept {
      _addr = rhs._addr;
      rhs._addr = nullptr;
    }
    Connector &Connector::operator=(srlib::net::Connector &&rhs) noexcept {
      _addr = rhs._addr;
      rhs._addr = nullptr;
      return *this;
    }
    std::shared_ptr<Connection> Connector::Connect() {
      return std::make_shared<TcpConnection>(*_addr);
    }
  }
}