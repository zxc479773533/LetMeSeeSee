/* 
 * pylib::io::pystring.h
 * 
 * Some string operate
 * 
 * Crated at 2019-01-23
 */

#ifndef PYLIB_PYSTRING
#define PYLIB_PYSTRING

#include <string>
#include <cstring>
#include <vector>
#include <regex>  

namespace pylib {
  // Here are some useful functions for string operation

  // Split's performance is like split in python
  inline std::vector<std::string> split(const std::string &str, const char delim) {
    std::vector<std::string> ret;
    std::stringstream ss(str);
    std::string data;
    while(std::getline(ss, data, delim))
      ret.push_back(data);
    return ret;
  }
  inline std::vector<std::string> split(const std::string &str, const char *delim) {
    int pos = 0;
    std::vector<std::string> ret;
    auto str_size = str.size();
    auto delim_size = strlen(delim);
    while (true) {
      if (pos >= str_size)
        break;
      auto delim_pos = str.find(delim, pos);
      ret.push_back(str.substr(pos, delim_pos - pos));
      if (delim_pos == std::string::npos)
        break;
      pos = delim_pos + delim_size;
    }
    return ret;
  }
  // CountSub is for counting the number of occurrences of a substring
  inline int CountSub(const std::string &str, const char substr) {
    int pos = 0;
    int ret = 0;
    auto str_size = str.size();
    auto sub_size = 1;
    while (true) {
      if (pos >= str_size)
        break;
      pos = str.find(substr, pos);
      if (pos == std::string::npos)
        break;
      ret++;
      pos += sub_size;
    }
    return ret;
  }
  inline int CountSub(const std::string &str, const char *substr) {
    int pos = 0;
    int ret = 0;
    auto str_size = str.size();
    auto sub_size = strlen(substr);
    while (true) {
      if (pos >= str_size)
        break;
      pos = str.find(substr, pos);
      if (pos == std::string::npos)
        break;
      ret++;
      pos += sub_size;
    }
    return ret;
  }
} // namespace pylib

#endif // !PYLIB_PYSTRING