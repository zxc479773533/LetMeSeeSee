#ifndef UTILITY_TLS_LISTENER_H
#define UTILITY_TLS_LISTENER_H

#include "listener.h"
struct ssl_st;
struct ssl_method_st;
struct ssl_ctx_st;
typedef struct ssl_st SSL;
typedef struct ssl_method_st SSL_METHOD;
typedef struct ssl_ctx_st SSL_CTX;
namespace srlib {
  namespace net {
    class TlsListener : public Listener {
      SSL* _ssl;
      SSL_CTX* _ctx;
      void Init();
    public:
    
    };
  }
}

#endif
