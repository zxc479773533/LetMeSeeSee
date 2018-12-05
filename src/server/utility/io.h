#ifndef SORANET_IO_H
#define SORANET_IO_H

#include <iostream>

#define ionosync() std::ios::sync_with_stdio(false)
#define iosync() std::ios::sync_with_stdio(true)
namespace srlib {
  inline void println() {
    std::cout << std::endl;
  }
  template<typename T>
  inline void println(T &&t) {
    std::cout << t << std::endl;
  }
  template<typename T, typename ...ARGS>
  inline void println(T &&t, ARGS &&... args) {
    std::cout << t << ' ';
    println(args...);
  }
  inline void logln() {
    std::clog << std::endl;
  }
  template<typename T>
  inline void logln(T &&t) {
    std::clog << t << std::endl;
  }
  template<typename T, typename ...ARGS>
  inline void logln(T &&t, ARGS &&... args) {
    std::clog << t << ' ';
    logln(args...);
  }
  template<typename Printable>
  inline void print(Printable &&p) {
    std::cout << p;
  }
  template<typename T, typename ...ARGS>
  inline void print(T &&t, ARGS &&... args) {
    std::cout << t << ' ';
    print(args...);
  };
  
  template<typename Hex>
  inline void printHex(Hex &&h) {
    std::cout << std::hex << h << std::dec;
  }
  inline void printHex(const void *mem, std::uint64_t size, std::uint64_t alignSize = 16) {
    auto hex = (std::uint8_t *) mem;
    auto start = (std::uint64_t) mem;
    auto pos = 0;
    auto align = pos % alignSize;
    if (align > 0) {
      auto padding = alignSize - align;
      printf("0x%lx: ", start - padding);
      for (int i = 0; i < padding; i++) {
        putchar('\t');
      }
      for (int i = 0; i < align; i++) {
        printf("%02x ", hex[pos++]);
      }
    } else {
      printf("0x%lx: ", start + pos);
    }
    for (; pos < size;) {
      printf("%02x ", hex[pos++]);
      if (pos % alignSize == align && pos < size) {
        printf("\n0x%lx: ", start + pos);
      }
    }
  }
  inline void printHexWithAscii(const void *mem, std::uint64_t size, std::uint64_t alignSize = 16) {
    auto hex = (std::uint8_t *) mem;
    auto start = (std::uint64_t) mem;
    auto pos = 0;
    auto align = pos % alignSize;
    if (align > 0) {
      auto padding = alignSize - align;
      printf("0x%lx: ", start - padding);
      for (int i = 0; i < padding; i++) {
        putchar('\t');
      }
      for (int i = 0; i < align; i++) {
        printf("%02x ", hex[pos + i]);
      }
      putchar('\t');
      for (int i = 0; i < align; i++) {
        char c = hex[pos + i];
        if (c <= 0x20 || c >= 0x7f) {
          putchar('.');
          continue;
        }
        putchar(hex[pos + i]);
      }
      pos += align;
    } else {
      printf("0x%lx: ", start + pos);
    }
    for (; pos < size;) {
      for (int i = 0; i < alignSize; i++) {
        printf("%02x ", hex[pos + i]);
      }
      putchar('\t');
      for (int i = 0; i < alignSize; i++) {
        char c = hex[pos + i];
        if (c <= 0x20 || c >= 0x7f) {
          putchar('.');
          continue;
        }
        putchar(hex[pos + i]);
      }
      pos += alignSize;
      if (pos >= size)break;
      printf("\n0x%lx: ", start + pos);
    }
  }
  template<typename Oct>
  inline void printOct(Oct &&o) {
    std::cout << std::oct << o << std::dec;
  }
  inline void scanln(std::string &s) {
    std::getline(std::cin, s);
  }
  template<typename Scanable>
  inline void scan(Scanable &s) {
    std::cin >> s;
  }
  template<typename Scanable>
  inline Scanable scan() {
    Scanable s;
    std::cin >> s;
    return s;
  }
  template<typename Hex>
  inline Hex scanHex() {
    Hex h;
    std::cin >> std::hex >> h;
    return h;
  }
  template<typename Oct>
  inline Oct scanOct() {
    Oct o;
    std::cin >> std::oct >> o;
    return o;
  }
  inline int scanHex() {
    int i;
    std::cin >> std::hex >> i;
    return i;
  }
  inline int scanOct() {
    int i;
    std::cin >> std::oct >> i;
    return i;
  }
}
#endif
