#include "address.h"
#include "utility/endiantr.h"
#include "utility/exstring.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

namespace srlib {
  namespace net {
#define v6 ((::sockaddr_in6*)(_addr))
#define v4 ((::sockaddr_in*)(_addr))
    Address::Address() : _addr(new sockaddr), _bad(true), _ipv6(false) {}
    Address::Address(const String &ip, std::uint16_t port, bool isIpv6)noexcept {
      _addr = new sockaddr;
      memset(_addr, 0, sizeof(sockaddr));
      _ipv6 = isIpv6;
      if (isIpv6) {
        v6->sin6_port = srlib::toBigEndian(port);
        v6->sin6_family = AF_INET6;
        auto status = ::inet_pton(AF_INET6, ip.c_str(), &v6->sin6_addr);
        if (status < 1)_bad = true;
      } else {
        v4->sin_port = srlib::toBigEndian(port);
        v4->sin_family = AF_INET;
        auto status = ::inet_pton(AF_INET, ip.c_str(), &(v4->sin_addr));
        if (status < 1)_bad = true;
      }
    }
    Address::Address(const String &ip, const String &port, bool isIpv6)noexcept
      : Address(ip, std::atoi(port.c_str()), isIpv6) {}
    Address::Address(const ::sockaddr &addr)noexcept : _bad(false) {
      _addr = new ::sockaddr;
      _bad = false;
      _ipv6 = addr.sa_family == AF_INET6;
      memmove(_addr, &addr, sizeof(addr));
    }
    Address &Address::operator=(const ::sockaddr &addr)noexcept {
      _addr = new ::sockaddr;
      memmove(_addr, &addr, sizeof(addr));
      return *this;
    }
    Address::Address(srlib::net::Address &&old) noexcept {
      _addr = old._addr;
      old._addr = nullptr;
      _ipv6 = old._ipv6;
      _bad = old._bad;
    }
    Address &Address::operator=(srlib::net::Address &&old) noexcept {
      _addr = old._addr;
      old._addr = nullptr;
      _ipv6 = old._ipv6;
      _bad = old._bad;
      return *this;
    }
    Address::Address(const Address &rhs) {
      _addr = new ::sockaddr;
      memmove(_addr, rhs._addr, sizeof(::sockaddr));
      _ipv6 = rhs._ipv6;
      _bad = rhs._bad;
    }
    Address::~Address() {
      delete _addr;
    }
    String Address::Ip() const {
      char buf[64];
      _ipv6 ? ::inet_ntop(AF_INET6, &v6->sin6_addr, buf, 64) : ::inet_ntop(AF_INET, &v4->sin_addr, buf, 64);
      return String(buf);
    }
    std::uint16_t Address::Port() const {return srlib::toBigEndian(v4->sin_port);}
    Address &Address::SetPort(std::uint16_t port) {
      if (_ipv6) {
        v6->sin6_port = toBigEndian(port);
      } else {
        v4->sin_port = toBigEndian(port);
      }
      return *this;
    }
    Address &Address::SetPort(const srlib::String &port) {
      if (_ipv6) {
        v6->sin6_port = std::uint16_t(port.to_integer());
      } else {
        v4->sin_port = std::uint16_t(port.to_integer());
      }
      return *this;
    }
    ::sockaddr *Address::Sockaddr() {return _addr;}
    const ::sockaddr *Address::Sockaddr() const {return _addr;}
    bool Address::IsIpv6() const {return _ipv6;}
    bool Address::IsBad() const {return _bad;}
    int Address::Family() const {return _ipv6 ? AF_INET6 : AF_INET;}
    void Address::SetSockaddr(sockaddr *addr) {memmove(&_addr, addr, sizeof(sockaddr));}
#undef v4
#undef v6
  }
}