#ifndef SORANET_ENDIANTR_H
#define SORANET_ENDIANTR_H

namespace srlib {
  template<typename INTTYPE>
  inline INTTYPE toBigEndian(INTTYPE i) {
#if LITTLE_ENDIAN == BYTE_ORDER
    INTTYPE res = 0;
    for (int it = 0; it < sizeof(INTTYPE); it++) {
      res <<= 8;
      res |= (i & 0xff);
      i >>= 8;
    }
    return res;
#endif
    return i;
  }
  template<typename INTTYPE>
  inline INTTYPE toLittleEndian(INTTYPE i) {
#if BIG_ENDIAN == BYTE_ORDER
    INTTYPE res = 0;
    for (int it = 0; it < sizeof(INTTYPE); it++) {
      res <<= 8;
      res |= (i & 0xff);
      i >>= 8;
    }
    return res;
#endif
    return i;
  }
}

#endif
