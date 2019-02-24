/* 
 * pylib::debug.h
 * 
 * For debug or output data
 * 
 * Crated at 2019-01-20
 */

#ifndef PYLIB_DEBUG
#define PYLIB_DEBUG

#include <iostream>

namespace pylib {
  // DEBUG MESSAGE FUNCTIONS
  // Arg is C style string

  // Print success message
  inline void succ_msg(const char *err) {
    printf("[SUCCEED] %s\n", err);
  }
  // Print error message
  inline void err_msg(const char *err) {
    printf("[ERROR] %s\n", err);
  }
  // Print usage
  inline void usage_err(const char *err) {
    printf("Usage: %s\n", err);
  }
  // Print error message and exit
  inline void err_exit(const char *err) {
    printf("[FATAL] %s\n", err);
    exit(1);
  }

  // println - Output the Arg
  inline void println() {
    std::cout << std::endl;
  }

  template<typename T, typename ...ARGS>
  inline void println(const T &first, const ARGS ...last) {
    std::cout << first << " ";
    println(last...);
  }

  // pylog - Output logs
  inline void pylog() {
    std::clog << std::endl;
  }

  template<typename T, typename ...ARGS>
  inline void pylog(const T &first, const ARGS ...last) {
    std::clog << first << " ";
    pylog(last...);
  }
} // namespace pylib

#endif // !PYLIB_DEBUG