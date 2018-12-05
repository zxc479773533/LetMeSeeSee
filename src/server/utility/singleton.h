#ifndef UTILITY_SINGLETON_H
#define UTILITY_SINGLETON_H

namespace srlib {
  /// Singleton Pattern.
  /// With C++11, static variable's construction is thread-safety.
  template<typename T>
  class Singleton {
  public:
    static T &GetInstance() {
      static T ins{};
      return ins;
    }
  };
}

#endif
