#include "../storager/storager.h"

int main() {
  lmss::Storager storager;
  storager.ListenAndServe("0.0.0.0", 11111);
}