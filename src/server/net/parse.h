#ifndef UTILITY_URL_PARSER_H
#define UTILITY_URL_PARSER_H

namespace srlib {
  class String;
  namespace net {
    class Address;
    Address ParseIp(const String &url, const String &service);
  }
}

#endif
