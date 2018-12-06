#ifndef UTILITY_ADDRESS_H
#define UTILITY_ADDRESS_H

#include <cstdint>

namespace srlib {
  class String;
}

struct sockaddr;
namespace srlib {
  namespace net {
    /// Address is the abstraction of struct sockaddr.
    class Address {
    private:
#define v6 ((::sockaddr_in6*)_addr)
#define v4 ((::sockaddr_in*)_addr)
      ::sockaddr *_addr;
      bool _bad;
      bool _ipv6;
    public:
      Address();
      Address(const Address &rhs);
      Address(const String &ip, std::uint16_t port, bool isIpv6 = false) noexcept;
      Address(const String &ip, const String &port, bool isIpv6 = false) noexcept;
      Address(const ::sockaddr &addr)noexcept;
      Address &operator=(const ::sockaddr &addr)noexcept;
      Address(Address &&old)noexcept;
      Address &operator=(Address &&old)noexcept;
      ~Address();
      String Ip() const;
      Address &SetPort(std::uint16_t port);
      Address &SetPort(const String &port);
      std::uint16_t Port() const;
      ::sockaddr *Sockaddr();
      const ::sockaddr *Sockaddr() const;
      bool IsIpv6() const;
      bool IsBad() const;
      int Family() const;
      void SetSockaddr(sockaddr *addr);
#undef v4
#undef v6
    };
  }
}

#endif
