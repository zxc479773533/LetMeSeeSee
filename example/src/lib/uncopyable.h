/* 
 * pylib::uncopyable.h
 * 
 * Let a class uncopyable
 * 
 * Crated at 2019-01-20
 */

#ifndef PYLIB_UNCOPYABLE
#define PYLIB_UNCOPYABLE

namespace pylib {
  class Uncopyable {
  protected:
    Uncopyable() {} // Allow construction and destruction
    ~Uncopyable() {}
  private:
    Uncopyable(const Uncopyable &); // Prohibited copy
    Uncopyable &operator=(const Uncopyable &);
  };
} // namespace pylib

#endif // !PYLIB_UNCOPYABLE