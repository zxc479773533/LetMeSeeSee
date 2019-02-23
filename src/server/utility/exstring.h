#ifndef UTILITY_NSTR_H
#define UTILITY_NSTR_H

#include <string>
#include <ostream>
#include <regex>

namespace srlib {
  template<typename T>
  struct remove_cvref {
    typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
  };
  template<typename T> using remove_cvref_t = typename remove_cvref<T>::type;
  
  template<typename CharT>
  class BasicString;
  
  /// BasicString extend std::basic_string<CharT>.
  /// operator-(str): remove str from tail.
  /// operator()(from,to): equal to substr(from,to-from), if from larger than size(), it will return empty.
  /// split() -> vector<BasicString>.
  /// substitute(old,new) -> replace all old to new in this string.
  /// regex_match(), regex_search(), regex_replace().
  /// map().
  /// trim().
  /// count().
  /// to_lower(), to_upper().
  /// has_prefix(), has_suffix().
  /// to_integer(), to_double().
  /// operator""_s.
  template<typename CharT>
  class BasicString {
#define DIFFTYPE_ENABLE template<typename T,typename Valid = typename std::enable_if<!std::is_same<BasicString, typename std::decay<T>::type>::value>::type>
#define CHAR_ENABLE template<typename Enable = typename std::enable_if<std::is_same<CharT,char>::value>::type>
  private:
    std::basic_string<CharT> _str;
  public:
    //Iterator
    typedef typename std::basic_string<CharT>::iterator iterator;
    typedef typename std::basic_string<CharT>::const_iterator const_iterator;
    typedef typename std::basic_string<CharT>::const_reverse_iterator const_reverse_iterator;
    typedef typename std::basic_string<CharT>::reverse_iterator reverse_iterator;
    //Constructor, Destructor
    ~BasicString() = default;
    BasicString() = default;
    BasicString(const CharT *str) : _str(str) {}
    BasicString(const CharT *str, size_t size) : _str(str, size) {}
    BasicString(std::basic_string<CharT> str) : _str(std::move(str)) {}
    BasicString(const BasicString &rhs) : _str(rhs._str) {}
    BasicString &operator=(const std::basic_string<CharT> &rhs) {
      _str = rhs;
      return *this;
    }
    BasicString &operator=(const BasicString &rhs) {
      _str = rhs._str;
      return *this;
    }
    BasicString &operator=(std::basic_string<CharT> &&rhs) {
      _str = std::move(rhs);
      return *this;
    }
    BasicString &operator=(BasicString &&rhs)noexcept {
      _str = std::move(rhs._str);
      return *this;
    }
    //Interaction with std::basic_string<CharT>
    inline std::basic_string<CharT> &std_string() {return _str;}
    inline std::basic_string<CharT> std_string_copy() {return _str;}
    inline const std::basic_string<CharT> &const_std_string() const {return _str;}
    inline const std::basic_string<CharT> const_std_string_copy() const {return _str;}
    inline const CharT *c_str() const {return _str.c_str();}
    inline const CharT *data() const {return _str.data();}
    inline CharT &at(size_t pos) {return _str.at(pos);}
    inline const CharT &at(size_t pos) const {return _str.at(pos);}
    inline CharT &operator[](int i) {return _str[i];}
    inline const CharT &operator[](int i) const {return _str[i];}
    inline CharT &front() {return _str.front();}
    inline const CharT &front() const {return _str.front();}
    inline CharT &back() {return _str.back();}
    inline const CharT &back() const {return _str.back();}
    inline iterator begin() {return _str.begin();}
    inline const_iterator begin() const {return _str.begin();}
    inline const_iterator cbegin() const {return _str.cbegin();}
    inline iterator end() {return _str.end();}
    inline const_iterator end() const {return _str.end();}
    inline const_iterator cend() const {return _str.cend();}
    inline reverse_iterator rbegin() {return _str.rbegin();}
    inline const_reverse_iterator rbegin() const {return _str.rbegin();}
    inline const_reverse_iterator crbegin() const {return _str.crbegin();}
    inline reverse_iterator rend() {return _str.rend();}
    inline const_reverse_iterator rend() const {return _str.rend();};
    inline const_reverse_iterator crend() const {return _str.crend();}
    inline bool empty() const {return _str.empty();}
    inline size_t size() const {return _str.size();}
    inline size_t length() const {return _str.length();}
    inline size_t max_size() const {return _str.max_size();}
    inline BasicString substr(size_t begin, size_t size) const {return BasicString(_str.substr(begin, size));}
    inline void reserve(size_t size) {_str.reserve(size);}
    inline size_t capacity() const {return _str.capacity();}
    inline void shrink_to_fit() {_str.shrink_to_fit();}
    inline void clear() {_str.clear();}
    template<typename ...ARGS>
    inline BasicString &insert(ARGS &&...args) {
      _str.insert(args...);
      return *this;
    }
    template<typename ...ARGS>
    inline BasicString insert_copy(ARGS &&...args) const {return BasicString(_str).insert(args...);}
    template<typename ...ARGS>
    inline BasicString &append(ARGS &&...args) {
      _str.append(args...);
      return *this;
    }
    template<typename ...ARGS>
    inline BasicString append_copy(ARGS &&...args) const {return BasicString(_str).append(args...);}
    template<typename ...ARGS>
    inline BasicString &erase(ARGS &&...args) {
      _str.erase(args...);
      return *this;
    }
    template<typename ...ARGS>
    inline BasicString erase_copy(ARGS &&...args) const {return BasicString(_str).erase(args...);}
    template<typename ...ARGS>
    inline size_t find(ARGS &&...args) const {return _str.find(args...);}
    template<typename ...ARGS>
    inline size_t rfind(ARGS &&...args) const {return _str.rfind(args...);}
    template<typename ...ARGS>
    inline size_t find_first_of(ARGS &&...args) const {return _str.find_first_of(args...);}
    template<typename ...ARGS>
    inline size_t find_first_not_of(ARGS &&...args) const {return _str.find_first_not_of(args...);}
    template<typename ...ARGS>
    inline size_t find_last_of(ARGS &&...args) const {return _str.find_last_of(args...);}
    template<typename ...ARGS>
    inline size_t find_last_not_of(ARGS &&...args) const {return _str.find_last_not_of(args...);}
    template<typename ...ARGS>
    inline BasicString &replace(ARGS &&...args) {
      _str.replace(args...);
      return *this;
    }
    template<typename ...ARGS>
    inline BasicString replace_copy(ARGS &&...args) const {return BasicString(_str).replace(args...);}
    inline void swap(BasicString &rhs) {_str.swap(rhs._str);}
    DIFFTYPE_ENABLE friend BasicString operator+(const BasicString &lhs, T &&rhs) {
      return BasicString(lhs._str + rhs);
    }
    DIFFTYPE_ENABLE friend BasicString operator+(T &&lhs, const BasicString &rhs) {
      return BasicString(lhs + rhs._str);
    };
    inline friend BasicString operator+(const BasicString &lhs, const BasicString &rhs) {
      return BasicString(lhs._str + rhs._str);
    }
    DIFFTYPE_ENABLE inline BasicString &operator+=(T &&rhs) {
      _str += rhs;
      return *this;
    }
    inline BasicString &operator+=(const BasicString &rhs) {
      _str += rhs._str;
      return *this;
    }
    DIFFTYPE_ENABLE inline friend BasicString operator-(const BasicString &lhs, T &&rhs) {
      return lhs.substr(0, lhs._str.rfind(rhs));
    }
    DIFFTYPE_ENABLE inline friend BasicString operator-(T &&lhs, const BasicString &rhs) {
      return BasicString(lhs) - rhs;
    }
    inline friend BasicString operator-(const BasicString &lhs, const BasicString &rhs) {
      return lhs.substr(0, lhs._str.rfind(rhs._str));
    }
    //FIXME: Bad Performance
    inline BasicString &operator-=(const BasicString &rhs) {
      auto pos = _str.rfind(rhs._str);
      if (pos != size_t(-1))
        _str.erase(pos);
      return *this;
    }
    DIFFTYPE_ENABLE inline BasicString &operator-=(T &&rhs) {
      auto pos = _str.rfind(rhs);
      if (pos != size_t(-1))
        _str.erase(pos);
      return *this;
    }
    inline bool operator<(const BasicString &rhs) const {
      return _str < rhs._str;
    }
    inline bool operator>(const BasicString &rhs) const {
      return rhs < *this;
    }
    inline bool operator<=(const BasicString &rhs) const {
      return !(rhs < *this);
    }
    inline bool operator>=(const BasicString &rhs) const {
      return !(*this < rhs);
    }
    bool operator==(const BasicString &rhs) const {
      return _str == rhs._str;
    }
    bool operator!=(const BasicString &rhs) const {
      return !(rhs == *this);
    }
    DIFFTYPE_ENABLE inline bool operator<(T &&rhs) const {
      return _str < rhs;
    }
    DIFFTYPE_ENABLE inline bool operator>(T &&rhs) const {
      return rhs < _str;
    }
    DIFFTYPE_ENABLE inline bool operator<=(T &&rhs) const {
      return !(rhs < _str);
    }
    DIFFTYPE_ENABLE inline bool operator>=(T &&rhs) const {
      return !(_str < rhs);
    }
    DIFFTYPE_ENABLE bool operator==(T &&rhs) const {
      return _str == rhs;
    }
    DIFFTYPE_ENABLE bool operator!=(T &&rhs) const {
      return !(rhs == _str);
    }
    friend std::ostream &operator<<(std::ostream &os, const BasicString &BasicString) {
      os << BasicString._str;
      return os;
    }
    //Extend
    template<typename Sub>
    bool contains(Sub &&sub) const {return _str.find(sub) != size_t(-1);}
    inline BasicString operator()(size_t begin, size_t end) const {
      if (begin == std::basic_string<CharT>::npos)return BasicString();
      if (begin >= end)return BasicString();
      return BasicString(_str.substr(begin, end - begin));
    }
    inline BasicString &trim_left(const BasicString &charset, size_t pos) {
      auto index1 = _str.find_first_not_of(charset._str, pos);
      if (index1 != std::basic_string<CharT>::npos)
        _str.erase(0, index1);
      return *this;
    }
    DIFFTYPE_ENABLE inline BasicString &trim_left(T &&charset, size_t pos) {
      auto index1 = _str.find_first_not_of(charset, pos);
      if (index1 != std::basic_string<CharT>::npos)
        _str.erase(0, index1);
      return *this;
    }
    inline BasicString trim_left_copy(const BasicString &charset, size_t pos = 0) const {
      auto index1 = _str.find_first_not_of(charset._str, pos);
      if (index1 == std::basic_string<CharT>::npos)
        return BasicString();
      return operator()(index1, size());
    }
    DIFFTYPE_ENABLE inline BasicString trim_left_copy(T &&charset, size_t pos = 0) const {
      auto index1 = _str.find_first_not_of(charset, pos);
      if (index1 == std::basic_string<CharT>::npos)
        return BasicString();
      return operator()(index1, size());
    }
    inline BasicString &trim_right(const BasicString &charset, size_t pos = std::basic_string<CharT>::npos) {
      auto index2 = _str.find_last_not_of(charset._str, pos);
      if (index2 == std::basic_string<CharT>::npos)
        _str.erase(index2 + 1);
      return *this;
    }
    DIFFTYPE_ENABLE inline BasicString &trim_right(T &&charset, size_t pos = std::basic_string<CharT>::npos) {
      auto index2 = _str.find_last_not_of(charset, pos);
      if (index2 == std::basic_string<CharT>::npos)
        _str.erase(index2 + 1);
      return *this;
    }
    inline BasicString trim_right_copy(const BasicString &charset, size_t pos = std::basic_string<CharT>::npos) const {
      auto index2 = _str.find_last_not_of(charset._str, pos);
      if (index2 == std::basic_string<CharT>::npos)
        return BasicString();
      return operator()(0, index2 + 1);
    }
    DIFFTYPE_ENABLE inline BasicString trim_right_copy(T &&charset, size_t pos = std::basic_string<CharT>::npos) const {
      auto index2 = _str.find_last_not_of(charset._str, pos);
      if (index2 == std::basic_string<CharT>::npos)
        return BasicString();
      return operator()(0, index2 + 1);
    }
    inline BasicString &trim(const char *charset = "\r\t\n \v",
                             size_t pos1 = 0,
                             size_t pos2 = std::basic_string<CharT>::npos) {
      return trim_left(charset, pos1).trim_right(charset, pos2);
    }
    inline BasicString &trim(const BasicString &charset,
                             size_t pos1 = 0,
                             size_t pos2 = std::basic_string<CharT>::npos) {
      return trim_left(charset, pos1).trim_right(charset, pos2);
    }
    DIFFTYPE_ENABLE inline BasicString &trim(T &&charset,
                                             size_t pos1 = 0,
                                             size_t pos2 = std::basic_string<CharT>::npos) {
      return trim_left(charset, pos1).trim_right(charset, pos2);
    }
    inline BasicString trim_copy(const char *charset = "\r\t\n \v",
                                 size_t pos1 = 0,
                                 size_t pos2 = std::basic_string<CharT>::npos) const {
      auto index1 = _str.find_first_not_of(charset, pos1);
      if (index1 == std::basic_string<CharT>::npos)
        return BasicString();
      auto index2 = _str.find_last_not_of(charset, pos2);
      if (index2 == std::basic_string<CharT>::npos)
        return BasicString();
      return operator()(index1, index2 + 1);
    }
    inline BasicString trim_copy(const BasicString &charset,
                                 size_t pos1 = 0,
                                 size_t pos2 = std::basic_string<CharT>::npos) const {
      auto index1 = _str.find_first_not_of(charset._str, pos1);
      if (index1 == std::basic_string<CharT>::npos)
        return BasicString();
      auto index2 = _str.find_last_not_of(charset._str, pos2);
      if (index2 == std::basic_string<CharT>::npos)
        return BasicString();
      return operator()(index1, index2 + 1);
    }
    DIFFTYPE_ENABLE inline BasicString trim_copy(T &&charset,
                                                 size_t pos1 = 0,
                                                 size_t pos2 = std::basic_string<CharT>::npos) const {
      auto index1 = _str.find_first_not_of(charset._str, pos1);
      if (index1 == std::basic_string<CharT>::npos)
        return BasicString();
      auto index2 = _str.find_last_not_of(charset._str, pos2);
      if (index2 == std::basic_string<CharT>::npos)
        return BasicString();
      return operator()(index1, index2 + 1);
    }
    inline BasicString &remove(size_t from, size_t to) {
      if (from < size()) {
        _str.erase(from, to - from);
      }
      return *this;
    }
    inline BasicString remove_copy(size_t from, size_t to) const {
      return BasicString(_str).remove(from, to);
    }
    inline BasicString &pop(size_t size) {
      _str.erase(_str.size() - size, size);
      return *this;
    }
    inline BasicString pop_copy(size_t size) const {
      return BasicString(_str).pop(size);
    }
    std::vector<BasicString> split(const BasicString &deli) const {
      unsigned long i = 0;
      unsigned long j = 0;
      std::vector<BasicString> res;
      auto strSize = _str.size();
      auto deliSize = deli.size();
      for (;;) {
        j = _str.find(deli._str, i);
        if (i < strSize && i < j)res.push_back(_str.substr(i, j - i));
        if (j == std::basic_string<CharT>::npos)break;
        i = j + deliSize;
      }
      return res;
    }
    std::vector<BasicString> split(const std::basic_string<CharT> &deli) const {
      unsigned long i = 0;
      unsigned long j = 0;
      std::vector<BasicString> res;
      auto strSize = _str.size();
      auto deliSize = deli.size();
      for (;;) {
        j = _str.find(deli, i);
        if (i < strSize && i < j)res.push_back(_str.substr(i, j - i));
        if (j == std::basic_string<CharT>::npos)break;
        i = j + deliSize;
      }
      return res;
    }
    std::vector<BasicString> split(const char *deli) const {
      unsigned long i = 0;
      unsigned long j = 0;
      std::vector<BasicString> res;
      auto strSize = _str.size();
      auto deliSize = strlen(deli);
      for (;;) {
        j = _str.find(deli, i);
        if (i < strSize && i < j)res.push_back(_str.substr(i, j - i));
        if (j == std::basic_string<CharT>::npos)break;
        i = j + deliSize;
      }
      return res;
    }
    std::vector<BasicString> split(const CharT deli) const {
      unsigned long i = 0;
      unsigned long j = 0;
      std::vector<BasicString> res;
      auto strSize = _str.size();
      auto deliSize = 1;
      for (;;) {
        j = _str.find(deli, i);
        if (i < strSize && i < j)res.push_back(_str.substr(i, j - i));
        if (j == std::basic_string<CharT>::npos)break;
        i = j + deliSize;
      }
      return res;
    }
    CHAR_ENABLE std::vector<BasicString> filed() const {
      auto size = _str.size();
      auto cstr = _str.c_str();
      std::vector<BasicString> res;
      unsigned pos = 0;
      unsigned start = 0;
      while (pos < size && isspace(cstr[pos]) != 0)pos++;
      start = pos;
      for (; pos < size;) {
        while (isspace(cstr[pos]) == 0)pos++;
        res.push_back(_str.substr(start, pos - start));
        while (pos < size && isspace(cstr[pos]) != 0)pos++;
        start = pos;
      }
      return res;
    }
    size_t count(const BasicString &sub) const {
      size_t res = 0;
      unsigned long pos = 0;
      auto size = sub.size();
      while (true) {
        pos = _str.find(sub._str, pos);
        if (pos == std::basic_string<CharT>::npos) {
          break;
        }
        pos += size;
        res++;
      }
      return res;
    }
    size_t count(const std::basic_string<CharT> &sub) const {
      size_t res = 0;
      unsigned long pos = 0;
      auto size = sub.size();
      while (true) {
        pos = _str.find(sub, pos);
        if (pos == std::basic_string<CharT>::npos) {
          break;
        }
        pos += size;
        res++;
      }
      return res;
    }
    CHAR_ENABLE size_t count(const char *sub) const {
      size_t res = 0;
      unsigned long pos = 0;
      auto size = strlen(sub);
      while (true) {
        pos = _str.find(sub, pos);
        if (pos == std::basic_string<CharT>::npos) {
          break;
        }
        pos += size;
        res++;
      }
      return res;
    }
    size_t count(const CharT sub) const {
      size_t res = 0;
      unsigned long pos = 0;
      auto size = 1;
      while (true) {
        pos = _str.find(sub, pos);
        if (pos == std::basic_string<CharT>::npos) {
          break;
        }
        pos += size;
        res++;
      }
      return res;
    }
    template<typename MapFunc>
    BasicString &map(MapFunc &&mapfunc) {
      for (auto &c:_str) {
        c = mapfunc(c);
      }
      return *this;
    }
    template<typename MapFunc>
    BasicString map_copy(MapFunc &&mapfunc) const {
      BasicString res(*this);
      for (auto &c:res._str) {
        c = mapfunc(c);
      }
      return res;
    }
    CHAR_ENABLE BasicString &to_upper() {
      for (auto &c : _str) {
        if (islower(c)) {
          c += 'A' - 'a';
        }
      }
      return *this;
    }
    CHAR_ENABLE BasicString to_upper_copy() const {
      BasicString res(*this);
      for (auto &c : res._str) {
        if (islower(c)) {
          c += 'A' - 'a';
        }
      }
      return res;
    }
    CHAR_ENABLE BasicString &to_lower() {
      for (auto &c:_str) {
        if (isupper(c)) {
          c += 'a' - 'A';
        }
      }
      return *this;
    }
    CHAR_ENABLE BasicString to_lower_copy() const {
      BasicString res(*this);
      for (auto &c:res._str) {
        if (isupper(c)) {
          c += 'a' - 'A';
        }
      }
      return res;
    }
    std::vector<BasicString> regex_search(const BasicString &str) const {
      std::regex reg(str._str);
      std::smatch match;
      std::vector<BasicString> res;
      std::basic_string<CharT> buf(_str);
      while (std::regex_search(buf, match, reg)) {
        res.emplace_back(BasicString(std::move(match.str())));
        buf = match.suffix();
      }
      return res;
    }
    DIFFTYPE_ENABLE std::vector<BasicString> regex_search(T &&str) const {
      std::regex reg(str);
      std::smatch match;
      std::vector<BasicString> res;
      std::basic_string<CharT> buf(_str);
      while (std::regex_search(buf, match, reg)) {
        res.emplace_back(BasicString(std::move(match.str())));
        buf = match.suffix();
      }
      return res;
    }
    bool regex_match(const BasicString &str) const {
      std::regex reg(str.c_str());
      std::smatch match;
      return std::regex_match(_str, match, reg);
    }
    DIFFTYPE_ENABLE bool regex_match(T &&str) const {
      std::regex reg(str);
      std::smatch match;
      return std::regex_match(_str, match, reg);
    }
    BasicString regex_replace(const CharT *str, const CharT *rep) const {
      return BasicString(std::regex_replace(_str, std::regex(str), rep));
    }
    BasicString regex_replace(const std::basic_string<CharT> &str, std::basic_string<CharT> &rep) const {
      return BasicString(std::regex_replace(_str, std::regex(str), rep));
    }
    BasicString regex_replace(const BasicString &str, const BasicString &rep) const {
      return BasicString(std::regex_replace(_str, std::regex(str._str), rep._str));
    }
    inline bool has_prefix(const BasicString &pre) const {return _str.find(pre._str) == 0;}
    inline bool has_suffix(const BasicString &suf) const {return _str.find(suf._str) + suf._str.size() == _str.size();}
    BasicString &substitute(const BasicString &old,
                            const BasicString &rep,
                            size_t count = std::basic_string<CharT>::npos) {
      while (count-- > 0) {
        auto pos = _str.find(old._str);
        if (pos == std::basic_string<CharT>::npos) {
          break;
        }
        _str.replace(pos, old.size(), rep._str);
      }
      return *this;
    }
    BasicString &substitute(const std::basic_string<CharT> &old,
                            const std::basic_string<CharT> &rep,
                            size_t count = std::basic_string<CharT>::npos) {
      while (count-- > 0) {
        auto pos = _str.find(old);
        if (pos == std::basic_string<CharT>::npos) {
          break;
        }
        _str.replace(pos, old.size(), rep);
      }
      return *this;
    }
    BasicString &substitute(const CharT *old, const CharT *rep, size_t count = std::basic_string<CharT>::npos) {
      auto size = strlen(old);
      while (count-- > 0) {
        auto pos = _str.find(old);
        if (pos == std::basic_string<CharT>::npos) {
          break;
        }
        _str.replace(pos, size, rep);
      }
      return *this;
    }
    // BasicString Convert
    std::int64_t to_integer() const {
      std::int64_t res;
      sscanf(_str.c_str(), "%ld", &res);
      return res;
    };
    std::uint64_t to_unsigned_integer() const {
      std::uint64_t res;
      sscanf(_str.c_str(), "%lu", &res);
      return res;
    }
    float to_float() const {
      float res;
      sscanf(_str.c_str(), "%f", &res);
      return res;
    }
    double to_double() const {
      double res;
      sscanf(_str.c_str(), "%lf", &res);
      return res;
    }
#undef DIFFTYPE_ENABLE
#undef CHAR_ENABLE
  };
  
  typedef BasicString<char> String;
  typedef BasicString<wchar_t> WString;
  typedef BasicString<char16_t> U16String;
  typedef BasicString<char32_t> U32String;
};

template<typename CharT>
struct std::hash<srlib::BasicString<CharT>> {
  size_t operator()(const srlib::BasicString<CharT> &str) const {
    return std::hash<std::basic_string<CharT>>()(str.const_std_string());
  }
};

inline srlib::BasicString<char> operator "" _s(const char *str, size_t size) {
  return srlib::BasicString<char>(str, size);
}
#endif //UTILITY_NSTR_H
