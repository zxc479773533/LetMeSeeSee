#ifndef SERVER_ENCRYPT_H
#define SERVER_ENCRYPT_H

#include "array.h"
#include "endiantr.h"

namespace srlib {
  
  constexpr static unsigned r[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9, 14, 20, 5, 9, 14,
    20, 5, 9, 14, 20, 5, 9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10,
    15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
  constexpr static unsigned k[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613,
    0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6,
    0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681,
    0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085,
    0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82,
    0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
  
  // !!! Assume it run in little-endian system
  class MD5 {
  private:
    static Array<Byte> Padding(const std::string &buf) {
      auto padding = 56 - buf.size() % 64;
      
      if (padding <= 0)padding += 64;
      Array<Byte> result(buf.size() + padding + 8);
      std::copy(buf.begin(), buf.end(), result.begin());
      result[buf.size()] = 0x80;
      for (int i = 1; i < padding; ++i) {
        result[buf.size() + i] = 0x00;
      }
      *((size_t *) (result.Data() + buf.size() + padding)) = buf.size() * 8;
      return result;
    }
    static Array<Byte> Padding(Slice<char> buf) {
      auto padding = 56 - buf.Size() % 64;
      
      if (padding <= 0)padding += 64;
      Array<Byte> result(buf.Size() + padding + 8);
      std::copy(buf.begin(), buf.end(), result.begin());
      result[buf.Size()] = 0x80;
      for (int i = 1; i < padding; ++i) {
        result[buf.Size() + i] = 0x00;
      }
      *((size_t *) (result.Data() + buf.Size() + padding)) = buf.Size() * 8;
      return result;
    }
    static uint32_t RotateLeft(uint32_t word, uint32_t bits) {
      return ((word << bits) | (word) >> (32 - bits));
    }
    static std::string MainHash(Array<Byte> buf) {
      int h0 = 0x67452301;
      int h1 = 0xEFCDAB89;
      int h2 = 0x98BADCFE;
      int h3 = 0x10325476;
      for (int i = 0; i < buf.Size() / 64; ++i) {
        uint32_t chunk[16];
        for (int j = 0; j < 16; ++j) {
          chunk[j] = *(uint32_t *) (buf.Data() + i * 64 + j * 4);
        }
        int a = h0, b = h1, c = h2, d = h3;
        for (int j = 0; j < 64; ++j) {
          int f = 0, g = 0;
          if (j >= 0 && j < 16) {
            f = (b & c) | ((~b) & d);
            g = j;
          } else if (j >= 16 && j < 32) {
            f = (d & b) | ((~d) & c);
            g = (5 * j + 1) % 16;
          } else if (j >= 32 && j < 48) {
            f = b ^ c ^ d;
            g = (3 * j + 5) % 16;
          } else if (j >= 48 && j < 64) {
            f = c ^ (b | (~d));
            g = (7 * j) % 16;
          }
          int temp = d;
          d = c;
          c = b;
          b = RotateLeft((a + f + k[j] + chunk[g]), r[j]) + b;
          a = temp;
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
      }
      char str[33];
      sprintf(str, "%08x%08x%08x%08x", toBigEndian(h0), toBigEndian(h1), toBigEndian(h2), toBigEndian(h3));
      return std::string(str, 32);
    }
  public:
    static std::string Hash(const std::string &str) {
      return MainHash(Padding(str));
    }
    static std::string Hash(Slice<char> buf) {
      return MainHash(Padding(buf));
    }
  };
}

#endif
