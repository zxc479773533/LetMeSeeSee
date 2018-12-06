#ifndef UTILITY_LISTENER_H
#define UTILITY_LISTENER_H

#include <sys/socket.h>
#include "utility/file.h"

namespace srlib {
  class String;
  namespace net {
    class Address;
    
    class Connection;
    
    class Listener : public File {
    private:
      Address *_addr = nullptr;
    public:
      Listener() = default;
      ~Listener() override;
      Listener(const Address &addr, bool listen = true, int backlog = 10);
      Listener(const String &ip, std::uint16_t port, bool listen = true, int backlog = 10);
      Listener(const String &ip, const String &port, bool listen = true, int backlog = 10);
      Listener(const Listener &) = delete;
      Listener &operator=(const Listener &)= delete;
      Listener(Listener &&rhs)noexcept;
      Listener &operator=(Listener &&rhs)noexcept;
      int Listen(int backlog = 10) const;
      std::shared_ptr<Connection> Accept() const;
      const Address &GetAddress() const;
    };
    
    std::shared_ptr<Listener> Listen(const Address &addr);
  }
}

#endif
