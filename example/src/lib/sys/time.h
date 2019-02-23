/* 
 * pylib::sys::time.h
 * 
 * Systen time
 * 
 * Crated at 2019-02-08
 */

#ifndef PYLIB_TIME
#define PYLIB_TIME

#include <ctime>
#include <cstring>
#include <sys/time.h>

namespace pylib {
  // Clock is the abstraction of struct time
  // Clock provide some APIs to get the system's time
  // It can record last time for calculate time
  class Clock {
  private:
    time_t _t;
    time_t _oldt;
    struct tm *_tm;
    struct tm *_oldtm;
  public:
    Clock() : _t(::time(NULL)), _oldt(_t), _tm(::localtime(&_t)), _oldtm(_tm) {}
    ~Clock() = default;
    Clock(const Clock &clock) {
      _t = clock._t;
      _tm = clock._tm;
    }
    Clock(Clock &&clock) noexcept {
      _t = clock._t;
      _tm = clock._tm;
    }
    Clock& operator=(const Clock &clock) noexcept {
      _t = clock._t;
      _tm = clock._tm;
      return *this;
    }
    Clock& operator=(Clock &&clock) {
      _t = clock._t;
      _tm = clock._tm;
      return *this;
    }
    void Refresh() {
      _oldt = _t;
      _oldtm = _tm;
      _t = ::time(NULL);
      _tm = ::localtime(&_t);
    }
    time_t Gettime() {
      return _oldt;
    }
    time_t Now() {
      Refresh();
      return _t;
    }
    // Get time
    int getsecond() const { return _tm->tm_sec; }
    int getminute() const { return _tm->tm_min; }
    int gethour() const { return _tm->tm_hour; }
    int getday() const { return _tm->tm_mday; }
    int getmonth() const { return _tm->tm_mon + 1; }
    int getyear() const { return _tm->tm_year + 1900; }
    int getweekday() const { return (_tm->tm_wday == 0) ? 7: _tm->tm_wday; }
    int getyearday() const { return _tm->tm_yday; }
    // Calculate time
    virtual time_t TimeDiff() {
      return _t - _oldt;
    }
  };
  // Timer is the abstraction of struct timeval
  // Timer provides APIs to calculate accurate time difference
  class Timer {
  private:
    struct timeval *_tval;
  public:
    Timer() : _tval(new struct timeval) {
      ::gettimeofday(_tval, NULL);
    }
    ~Timer() {
      delete _tval;
    }
    Timer(const Timer &timer) {
      _tval = timer._tval;
    }
    Timer(Timer &&timer) noexcept {
      _tval = timer._tval;
      timer._tval = nullptr;
    }
    Timer& operator=(const Timer &timer) noexcept {
      _tval = timer._tval;
      return *this;
    }
    Timer& operator=(Timer &&timer) {
      _tval = timer._tval;
      timer._tval = nullptr;
      return *this;
    }
    // Get time
    time_t Second() const { return _tval->tv_sec; }
    long Microsecond() const { return _tval->tv_usec; }
    double DecimalSecond() const {
      double ret = _tval->tv_sec * 1000 + _tval->tv_usec;
      return ret / 1000;
    }
    // Calculate time
    Timer& Add(const Timer &timer) {
      _tval->tv_sec += timer._tval->tv_sec;
      _tval->tv_usec %= 1000;
      _tval->tv_usec += timer._tval->tv_usec + _tval->tv_usec / 1000;
      return *this;
    }
    Timer& operator+=(const Timer &timer) {
      _tval->tv_sec += timer._tval->tv_sec;
      _tval->tv_usec %= 1000;
      _tval->tv_usec += timer._tval->tv_usec + _tval->tv_usec / 1000;
      return *this;
    }
    Timer operator+(const Timer &timer) {
      Timer newtimer(*this);
      newtimer._tval->tv_sec += timer._tval->tv_sec;
      newtimer._tval->tv_usec %= 1000;
      newtimer._tval->tv_usec += timer._tval->tv_usec + _tval->tv_usec / 1000;
      return newtimer;
    }
    Timer& Sub(const Timer &timer) {
      if ((_tval->tv_sec * 1000 + _tval->tv_usec) < (timer._tval->tv_sec * 1000 + timer._tval->tv_usec)) {
        _tval->tv_sec = 0;
        _tval->tv_usec = 0;
      }
      else {
        long timediff = (_tval->tv_sec * 1000 + _tval->tv_usec) - (timer._tval->tv_sec * 1000 + timer._tval->tv_usec);
        _tval->tv_sec = timediff / 1000;
        _tval->tv_usec = timediff % 1000;
      }
      return *this;
    }
    Timer& operator-=(const Timer &timer) {
      if ((_tval->tv_sec * 1000 + _tval->tv_usec) < (timer._tval->tv_sec * 1000 + timer._tval->tv_usec)) {
        _tval->tv_sec = 0;
        _tval->tv_usec = 0;
      }
      else {
        long timediff = (_tval->tv_sec * 1000 + _tval->tv_usec) - (timer._tval->tv_sec * 1000 + timer._tval->tv_usec);
        _tval->tv_sec = timediff / 1000;
        _tval->tv_usec = timediff % 1000;
      }
      return *this;
    }
    Timer operator-(const Timer &timer) {
      Timer newtimer(*this);
      if ((newtimer._tval->tv_sec * 1000 + newtimer._tval->tv_usec) < (timer._tval->tv_sec * 1000 + timer._tval->tv_usec)) {
        newtimer._tval->tv_sec = 0;
        newtimer._tval->tv_usec = 0;
      }
      else {
        long timediff = (newtimer._tval->tv_sec * 1000 + newtimer._tval->tv_usec) - (timer._tval->tv_sec * 1000 + timer._tval->tv_usec);
        newtimer._tval->tv_sec = timediff / 1000;
        newtimer._tval->tv_usec = timediff % 1000;
      }
      return newtimer;
    }
  };
} // namespace pylib

#endif // !PYLIB_TIME