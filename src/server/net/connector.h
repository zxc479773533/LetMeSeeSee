#ifndef UTILITY_CONNECTOR_H
#define UTILITY_CONNECTOR_H

#include <memory>

namespace srlib {
  namespace net {
    class Connection;
    
    class UdpConnection;
    
    class Address;
    
    std::shared_ptr<Connection> Dial(const Address &addr);
    
    class Connector {
    private:
      Address *_addr;
    public:
      Connector() = default;
      ~Connector();
      Connector(const Address &addr);
      Connector(const Connector &rhs);
      Connector(Connector &&rhs)noexcept;
      Connector &operator=(const Connector &rhs);
      Connector &operator=(Connector &&rhs)noexcept;
      std::shared_ptr<Connection> Connect();
    };
  }
}

#endif
