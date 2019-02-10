#ifndef UTILITY_LOGGER_H
#define UTILITY_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include "array.h"

namespace srlib {
  namespace log {
    class TimeFormatter {
    private:
      Array<char> _m_buf;
      std::string _m_fmt;
    public:
      TimeFormatter() : _m_buf(256), _m_fmt("%c") {}
      void SetBufferSize(std::size_t size) {
        if (_m_buf.Size() == size)return;
        _m_buf = Array<char>(size);
      }
      std::string GetFormattedTime() {
        auto tm = std::time(nullptr);
        auto size = std::strftime(_m_buf.Data(), _m_buf.Size(), _m_fmt.c_str(), std::localtime(&tm));
        return std::string(_m_buf.Data(), size);
      }
      void SetTimeFormat(const char *fmt) {
        _m_fmt = fmt;
      }
    };
    
    class Logger {
    private:
      // Output Stream => default: std::clog
      std::ostream *_m_os;
      // Time Formatter
      TimeFormatter _m_time{};
      // Format Time Or Not => default: true
      bool _m_time_flag;
      // String Before Time And Message => default: ""
      std::string _m_prefix{};
      // String Between Time And Message => default: ": " (if _m_time_flag == false, logger will not print this string.)
      std::string _m_deli;
      // String After Message => default: ""
      std::string _m_suffix{};
    private:
      std::ostream &Os() {return *_m_os;}
      void LogPrefix() {Os() << _m_prefix;}
      void LogTime() {Os() << _m_time.GetFormattedTime() << _m_deli;}
      void LogMessage(const std::string &msg) {Os() << msg;}
      void LogSuffix() {Os() << _m_suffix;}
      void LogWithFlag(const std::string &msg, bool ln, bool fatal) {
        if (_m_os == nullptr)return;
        LogPrefix();
        if (_m_time_flag)LogTime();
        LogMessage(msg);
        LogSuffix();
        if (ln)Os() << std::endl;
        if (fatal)std::abort();
      }
    public:
      Logger() : _m_os(&std::clog), _m_time_flag(true), _m_deli(": ") {}
      ~Logger() {
        if (_m_os != &std::clog && _m_os != &std::cerr && _m_os != &std::cout) {
          delete _m_os;
          _m_os = nullptr;
        }
      }
      void SetOutputFile(const std::string &filename) {
        if (_m_os != &std::clog && _m_os != &std::cerr && _m_os != &std::cout) {
          delete _m_os;
          _m_os = nullptr;
        }
        _m_os = new std::ofstream(filename);
      }
      void DisableLogTime() {_m_time_flag = false;}
      void EnableLogTime() {_m_time_flag = true;}
      void SetTimeFormatterBufferSize(std::size_t size) {_m_time.SetBufferSize(size);}
      void SetTimeFormat(const char *fmt) {_m_time.SetTimeFormat(fmt);}
      void SetPrefix(const char *prefix) {_m_prefix = prefix;}
      void SetDeli(const char *deli) {_m_deli = deli;}
      void SetSuffix(const char *suffix) {_m_suffix = suffix;}
      void Log(const std::string &msg) {LogWithFlag(msg, false, false);}
      void Logln(const std::string &msg) {LogWithFlag(msg, true, false);}
      void Fatal(const std::string &msg) {LogWithFlag(msg, false, true);}
      void Fatalln(const std::string &msg) {LogWithFlag(msg, true, true);}
    };
    
    static Logger Log;
  }
}

#endif
