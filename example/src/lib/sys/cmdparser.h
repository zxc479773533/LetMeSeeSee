/* 
 * pylib::sys::cmdparser.h
 * 
 * Command line parser
 * 
 * Crated at 2019-01-23
 */

#ifndef PYLIB_CMDPARSER
#define PYLIB_CMDPARSER

#include "../uncopyable.h"
#include "../io/pystring.h"
#include <iostream>
#include <queue>

namespace pylib {
  class CmdParser : Uncopyable {
  private:
    int _opnum;
    int _pos;
    std::queue<std::string> _ops;
    std::queue<std::queue<std::string>> _args;
  public:
    CmdParser() = default;
    CmdParser(int argc, char **argv) {
      _pos = 0;
      _opnum = 0;
      for (int i = 1; i < argc; i++) {
        // Set operation
        if (strncmp(argv[i], "--", 2) == 0)
          _ops.push(std::string(argv[i]).substr(2));
        else if (strncmp(argv[i], "-", 1) == 0)
          _ops.push(std::string(argv[i]).substr(1));
        std::queue<std::string> tmp;
        _opnum++;
        i++;
        // Get args
        while (i < argc && argv[i][0] != '-')
          tmp.push(std::string(argv[i++]));
        // Set args
        _args.push(tmp);
        i--;
      }
    }
    ~CmdParser() {}
    CmdParser(CmdParser &&cmdparser) {
      _opnum = cmdparser._opnum;
      _pos = cmdparser._pos;
      _ops = std::move(cmdparser._ops);
      _args = std::move(cmdparser._args);
    }
    CmdParser& operator=(CmdParser &&cmdparser) {
      _opnum = cmdparser._opnum;
      _pos = cmdparser._pos;
      _ops = std::move(cmdparser._ops);
      _args = std::move(cmdparser._args);
      return *this;
    }
    // Get info
    int NowPos() { return _pos; }
    int OperateNum() { return _opnum; }
    // Next step
    std::string Next(std::queue<std::string> &arg_queue) {
      if (_pos < _opnum) {
        std::string op = _ops.front();
        arg_queue = _args.front();
        _ops.pop();
        _args.pop();
        _pos++;
        return op;
      }
      return "";
    }
    // Get value
  };
} // namespace pylib

#endif // !PYLIB_CMDPARSER