#ifndef UTILITY_PIPE_H
#define UTILITY_PIPE_H

#include "exstring.h"
#include "array.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/eventfd.h>
#include <dirent.h>
#include <sys/ioctl.h>

namespace srlib {
  class FileMode {
  private:
    mode_t _mode;
  public:
    FileMode(mode_t mode) : _mode(mode) {}
    bool IsDir() {return S_ISDIR(_mode);}
    inline bool IsRegular() const {return S_ISREG(_mode);}
    inline bool IsSocket() const {return S_ISSOCK(_mode);}
    inline bool IsLink() const {return S_ISLNK(_mode);}
    inline bool IsPipe() const {return S_ISFIFO(_mode);}
  };
  
  /// FileInfo include a file's information.
  class FileInfo {
  private:
    String _name;
    String _path;
    struct stat *_st;
  public:
    FileInfo() = default;
    ~FileInfo() {delete _st;}
    FileInfo(const String &dir, const String &name) : _name(name), _path(dir + '/' + name), _st(new struct stat) {
      if (::stat(_path.c_str(), _st) != 0) {
        //perror("stat");
        delete _st;
        _st = nullptr;
      }
    }
    FileInfo(const FileInfo &rhs) {
      _name = rhs._name;
      _path = rhs._path;
      _st = new struct stat;
      memmove(_st, rhs._st, sizeof(struct stat));
    }
    FileInfo(FileInfo &&rhs) {
      _name = std::move(rhs._name);
      _path = std::move(rhs._path);
      _st = rhs._st;
      rhs._st = nullptr;
    }
    FileInfo &operator=(const FileInfo &rhs) {
      _name = rhs._name;
      _path = rhs._path;
      _st = new struct stat;
      memmove(_st, rhs._st, sizeof(struct stat));
      return *this;
    }
    FileInfo &operator=(FileInfo &&rhs) {
      _name = std::move(rhs._name);
      _path = std::move(rhs._path);
      _st = rhs._st;
      rhs._st = nullptr;
      return *this;
    }
    //Status
    off_t Size() const {return _st->st_size;}
    const String &Name() const {return _name;}
    const String &Path() const {return _path;}
    inline FileMode Mode() const {return FileMode(_st->st_mode);}
    inline bool IsDir() const {return S_ISDIR(_st->st_mode);}
    inline bool IsRegular() const {return S_ISREG(_st->st_mode);}
    inline bool IsSocket() const {return S_ISSOCK(_st->st_mode);}
    inline bool IsLink() const {return S_ISLNK(_st->st_mode);}
    inline bool IsPipe() const {return S_ISFIFO(_st->st_mode);}
    inline bool IsValid() const {return _st != nullptr;}
    //Compare
    bool operator<(const FileInfo &rhs) const {return _name < rhs._name;}
  };
  
  /// File is the abstraction of file descriptor in Unix and Unix-like System.
  /// It can be a regular file, socket, pipe or others. File provides API to check its type.
  /// File's Constructor accepts a fd, and store it and its status to member.
  /// Basically File provides Read() and Write() to control the fd.
  /// File is not allowed to use Copy Constructor, because it's a ResourceManager Class.
  /// But you have to call Close() to close the file descriptor.
  class File {
  private:
    int _fd;
    struct stat *_st;
  public:
    //Constructor & Destructor
    File() = default;
    explicit File(int fd) : _fd(fd), _st(new struct stat) {
      if (fd < 0) {
        fprintf(stderr, "File Constructor Error: Fd < 0.\n");
      } else {
        if (::fstat(fd, _st) != 0) {
          perror("fstat");
        }
      }
    }
    virtual ~File() {delete _st;}
    File(const File &rhs) = delete;
    File(File &&old)noexcept {
      _fd = old._fd;
      old._fd = -1;
      _st = old._st;
      old._st = nullptr;
    }
    File &operator=(const File &rhs)= delete;
    File &operator=(File &&old)noexcept {
      _fd = old._fd;
      old._fd = -1;
      _st = old._st;
      old._st = nullptr;
      return *this;
    }
    //Get
    int fd() const {return _fd;}
    //Set
    void SetFd(int fd) {_fd = fd;}
    //I/O
    virtual String Read(size_t size) const {
      auto buf = new char[size];
      auto count = ::read(_fd, buf, size);
      String res(buf, count);
      delete buf;
      return res;
    }
    virtual ssize_t Read(Array<char> &buf) {
      return ::read(fd(), buf.Data(), buf.Size());
    }
    virtual ssize_t Read(Array<uint8_t> &buf) {
      return ::read(fd(), buf.Data(), buf.Size());
    }
    virtual ssize_t Read(Slice<char> buf) {
      return ::read(fd(), buf.Data(), buf.Size());
    }
    virtual ssize_t Read(Slice<uint8_t> buf) {
      return ::read(fd(), buf.Data(), buf.Size());
    }
    virtual ssize_t Write(const String &str) const {
      return ::write(_fd, str.c_str(), str.size());
    }
    virtual ssize_t Write(const Array<char> &buf) {
      return ::write(_fd, buf.Data(), buf.Size());
    }
    virtual ssize_t Write(const Array<uint8_t> &buf) {
      return ::write(_fd, buf.Data(), buf.Size());
    }
    virtual ssize_t Write(const Slice<char> &buf) {
      return ::write(_fd, buf.Data(), buf.Size());
    }
    virtual ssize_t Write(const Slice<uint8_t> &buf) {
      return ::write(_fd, buf.Data(), buf.Size());
    }
    virtual int Pending() {
      int pending;
      ioctl(_fd, FIONREAD, &pending);
      return pending;
    }
    int Close() const {return ::close(_fd);}
    //Status
    inline off_t Size() const {return _st->st_size;}
    inline bool IsValid() const {return _fd >= 0;}
    inline bool IsDir() const {return S_ISDIR(_st->st_mode);}
    inline bool IsRegular() const {return S_ISREG(_st->st_mode);}
    inline bool IsSocket() const {return S_ISSOCK(_st->st_mode);}
    inline bool IsLink() const {return S_ISLNK(_st->st_mode);}
    inline bool IsPipe() const {return S_ISFIFO(_st->st_mode);}
    inline bool IsNonBlock() const {return (fcntl(_fd, F_GETFL) & O_NONBLOCK) != 0;}
    inline FileMode Mode() const {return FileMode(_st->st_mode);}
    inline String FileName() const {
      static char buf[2048]{};
      auto count = ::readlink(("/proc/self/fd/" + std::to_string(_fd)).c_str(), buf, 2048);
      return String(buf, count);
    }
    inline String FileNameSafe() const {
      char buf[2048]{};
      auto count = ::readlink(("/proc/self/fd/" + std::to_string(_fd)).c_str(), buf, 2048);
      return String(buf, count);
    }
  };
  
  /// RegularFile is the abstraction of regular file.
  /// It is derived from File, so its Copy Constructor is also deleted.
  /// RegularFile is RAII class, it opens a file when construct, close the file when destruct.
  /// It provides ReadAll() to read all contents.
  class RegularFile : public File {
    String _path;
    String _name;
  public:
    //Constructor & Destructor
    RegularFile() = default;
    RegularFile(const String &filepath, int flag = O_RDWR | O_CREAT, int mode = 0666) : File(::open(filepath.c_str(),
                                                                                                    flag,
                                                                                                    mode)),
      _path(filepath) {
      if (_path.contains('/'))_name = _path(_path.rfind('/'), _path.size());
      else _name = _path;
    }
    ~RegularFile() {Close();}
    RegularFile(const RegularFile &rhs) = delete;
    RegularFile(RegularFile &&rhs)noexcept : File(std::move(rhs)) {
      _path = std::move(rhs._path);
      _name = std::move(rhs._name);
    };
    RegularFile &operator=(const RegularFile &rhs) = delete;
    RegularFile &operator=(RegularFile &&rhs)noexcept {
      _path = std::move(rhs._path);
      _name = std::move(rhs._name);
      static_cast<File &>(*this) = std::move(rhs);
      return *this;
    };
    //Utility IO
    String ReadAll() {return Read(Size());}
    //Status
    const String &Name() const {return _name;}
    const String &Path() const {return _path;}
  };
  
  /// OpenFile() is equal to RegularFile() if RVO is available.
  inline RegularFile OpenFile(const String &filepath, int flag = O_RDWR | O_CREAT, int mode = 0666) {
    return RegularFile(filepath);
  }
  /// ReadRegularFile is equal to RegularFile().ReadAll().
  inline String ReadRegularFile(const String &filepath) {return RegularFile(filepath).ReadAll();}
  
  /// Waker is the abstraction of eventfd.
  /// Call Wake() to write a 8 bytes integer and select, poll, epoll will return.
  /// Call Renew() to read a 8 bytes integer.
  class Waker : public File {
  private:
    std::uint64_t _buf;
  public:
    //Constructor
    Waker() : File(::eventfd(0, 0)), _buf(0) {}
    //Function
    //Wake Return Success or not.
    bool Wake(std::uint64_t num = 1) {return ::write(fd(), &num, sizeof(num)) == sizeof(num);}
    //Renew Return Value Read From Waker.
    std::uint64_t Renew() {
      ::read(fd(), &_buf, sizeof(_buf));
      return _buf;
    }
  };
  
  /// Directory is the abstraction of directory.
  /// It provides Read() to return a file's info in this directory.
  /// It is a RAII class. Open directory when construct, close directory when destruct.
  /// It provides Walk() and WalkRecursive() to apply a function to each file in this directory (and subdirectory).
  /// WalkFunc => function<void(FileInfo&&,ARGS...)>
  /// For example:
  /// map<int,FileInfo> im;
  /// auto f = [](FileInfo&& info, map<int,FileInfo>& info_map){};
  /// Directory(".").Walk(f,im);
  class Directory {
  private:
    DIR *_dir;
    int _fd;
    String _name;
  public:
    //Constructor
    Directory(const String &dirpath, bool create = false, mode_t mode = 0777)
      : _dir(::opendir(dirpath.c_str())), _name(dirpath) {
      if (_dir == nullptr && create) {
        ::mkdir(dirpath.c_str(), mode);
        _dir = ::opendir(dirpath.c_str());
      }
      if (_dir == nullptr)_fd = -1;
      else _fd = dirfd(_dir);
    }
    ~Directory() {
      ::closedir(_dir);
    }
    Directory(const Directory &) = delete;
    Directory &operator=(const Directory &)= delete;
    Directory(Directory &&rhs) noexcept {
      _dir = rhs._dir;
      _fd = rhs._fd;
      _name = std::move(rhs._name);
      rhs._dir = nullptr;
      rhs._fd = -1;
    }
    Directory &operator=(Directory &&rhs)noexcept {
      _dir = rhs._dir;
      _fd = rhs._fd;
      _name = std::move(rhs._name);
      rhs._dir = nullptr;
      rhs._fd = -1;
      return *this;
    }
    //Status
    inline bool IsValid() const {return _fd >= 0 && _dir != nullptr;}
    inline const String &Name() const {return _name;}
    inline int fd() const {return _fd;}
    //I/O
    FileInfo Read() const {
      auto file = ::readdir(_dir);
      if (file == nullptr) {
        return FileInfo();
      }
      return FileInfo(_name, file->d_name);
    }
    inline int Close() const {return ::closedir(_dir);}
    //Function
    //WalkFunc => std::function<void(FileInfo&&,ARGS&&)>.
    template<typename WalkFunc, typename ...ARGS>
    void Walk(WalkFunc &&fn, ARGS &&...args) const {
      while (true) {
        auto file = Read();
        if (file.Name().empty())break;
        if (!file.IsValid())continue;
        if (file.Name() == "." || file.Name() == "..")continue;
        fn(std::move(file), args...);
      }
    };
    template<typename WalkFunc, typename ...ARGS>
    void RecursiveWalk(WalkFunc &&fn, ARGS &&...args) const {
      while (true) {
        auto file = Read();
        if (file.Name().empty())break;
        if (!file.IsValid())continue;
        if (file.IsDir()) {
          if (file.Name() != "." && file.Name() != "..") {
            Directory next(file.Path());
            if (next.IsValid())next.RecursiveWalk(fn, args...);
          }
          continue;
        }
        fn(std::move(file), args...);
      }
    };
  };
  
  /// OpenDir() is equal to Directory().
  inline Directory OpenDir(const String &dirpath, bool create = false, mode_t mode = 0777) {
    return Directory(dirpath, create, mode);
  }
}

#endif
