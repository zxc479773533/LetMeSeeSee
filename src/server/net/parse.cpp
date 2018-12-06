#include "parse.h"
#include "address.h"
#include "utility/exstring.h"
#include <netdb.h>

namespace srlib {
  namespace net {
    Address ParseIp(const String &url, const String &service) {
      ::addrinfo hints{};
      ::addrinfo *res = nullptr;
      hints.ai_flags = 0;
      hints.ai_protocol = 0;
      hints.ai_addrlen = sizeof(sockaddr);
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_family = AF_INET;
      getaddrinfo(url.c_str(), service.c_str(), &hints, &res);
      for (auto ptr = res; ptr != nullptr; ptr = ptr->ai_next) {
        if (ptr->ai_family == AF_INET) {
          auto addr = *ptr->ai_addr;
          freeaddrinfo(res);
          return Address(addr);
        }
      }
      freeaddrinfo(res);
      return Address();
    }
  }
}