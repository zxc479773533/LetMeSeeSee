/* 
 * pylib::io::file.h
 * 
 * For file read/write
 * 
 * Crated at 2019-01-21
 */

#ifndef PYLIB_FILE
#define PYLIB_FILE

#include "../debug.h"
#include "../uncopyable.h"
#include "pystring.h"
#include <unistd.h>     // Unix system calls
#include <dirent.h>     // Directory defines
#include <fcntl.h>      // Micros for I/O
#include <sys/stat.h>   // System data type

namespace pylib {
  // FileInfo is the abstraction of file name, file path and struct stat
  // FileInfo's constructor accept's file's full path and name
  // FileInfo provide APIs to check its type and permiss
  // FileInfo overloaded operator > and < for compare
  class FileInfo {
  private:
    std::string _name;
    std::string _path;
    std::string _fullpath;
    struct stat *_st;
  public:
    FileInfo() = default;
    ~FileInfo() noexcept { delete _st; }
    FileInfo(const std::string &directory, const std::string &name)
     : _name(name), _path(directory + "/" + name), _st(new struct stat) {
      auto buf = new char[1024];
      ::realpath(_path.c_str(), buf);
      _fullpath = buf;
      delete[] buf;
      if (::stat(_path.c_str(), _st) != 0) {
        err_msg("stat failed: ret != 0");
        delete _st;
        _st = nullptr;
      }
    }
    FileInfo(const FileInfo &fileinfo) {
      _name = fileinfo._name;
      _path = fileinfo._path;
      _st = new struct stat;
      ::memmove(_st, fileinfo._st, sizeof(struct stat));
    }
    // Right value reference do not need to create new struct
    FileInfo(FileInfo &&fileinfo) noexcept {
      _name = std::move(fileinfo._name);
      _path = std::move(fileinfo._name);
      _st = fileinfo._st;
      fileinfo._st = nullptr;
    }
    FileInfo& operator=(const FileInfo &fileinfo) noexcept {
      _name = fileinfo._name;
      _path = fileinfo._path;
      _st = new struct stat;
      ::memmove(_st, fileinfo._st, sizeof(struct stat));
      return *this;
    }
    FileInfo& operator=(FileInfo &&fileinfo) {
      _name = std::move(fileinfo._name);
      _path = std::move(fileinfo._name);
      _st = fileinfo._st;
      fileinfo._st = nullptr;
      return *this;
    }
    bool operator<(const FileInfo &fileinfo) const {
      return _name < fileinfo._name;
    }
    bool operator>(const FileInfo &fileinfo) const {
      return _name > fileinfo._name;
    }
    // Get info
    const std::string& Name() const { return _name; }
    const std::string& Path() const { return _path; }
    const std::string& FullPath() const { return _fullpath; }
    // Get file size
    inline __off_t Size() const { return _st->st_size; }
    // Get file type
    inline bool IsDir() const { return S_ISDIR(_st->st_mode); }
    inline bool IsRegular() const { return S_ISREG(_st->st_mode); }
    inline bool IsPipe() const { return S_ISFIFO(_st->st_mode); }
    inline bool IsLink() const { return S_ISLNK(_st->st_mode); }
    inline bool IsChar() const { return S_ISCHR(_st->st_mode); }
    inline bool IsBlock() const { return S_ISBLK(_st->st_mode); }
    inline bool IsSocket() const { return S_ISSOCK(_st->st_mode); }
    // Get file permission
    inline bool CanUserRead() const { return _st->st_mode & S_IRUSR; }
    inline bool CanUserWrite() const { return _st->st_mode & S_IWUSR; }
    inline bool CanUserExecute() const { return _st->st_mode & S_IXUSR; }
    inline bool CanGroupRead() const { return _st->st_mode & S_IRGRP; }
    inline bool CanGroupWrite() const { return _st->st_mode & S_IWGRP; }
    inline bool CanGroupExecute() const { return _st->st_mode & S_IXGRP; }
    inline bool CanOtherRead() const { return _st->st_mode & S_IROTH; }
    inline bool CanOtherWrite() const { return _st->st_mode & S_IWOTH; }
    inline bool CanOtherExecute() const { return _st->st_mode & S_IXOTH; }
  };

  // File is the abstraction of file descripter
  // File inherited class Uncopyable to prohibit copy constructor
  // File provide APIs to check its type and permiss
  // File provide basic I/O operation to control the fd
  class File : Uncopyable {
  private:
    int _fd;
    struct stat *_st;
  public:
    File() = default;
    File(int fd) : _fd(fd), _st(new struct stat) {
      if (fd < 0)
        err_msg("Class File Constructor failed: fd < 0");
      else {
        if (::fstat(fd, _st) != 0) {
          err_msg("fstat failed: ret != 0");
          delete _st;
          _st = nullptr;
        }
      }
    }
    // The base class need virtual destructor
    virtual ~File() { delete _st; }
    File(File &&file) noexcept {
      _fd = std::move(file._fd);
      _st = file._st;
      file._st = nullptr;
    }
    File& operator=(File &&file) {
      _fd = std::move(file._fd);
      _st = file._st;
      file._st = nullptr;
      return *this;
    }
    // Get info
    int fd() const { return _fd; }
    const std::string FileName() const {
      char buf[1024] = {};
      ::readlink(("/proc/self/fd/" + std::to_string(_fd)).c_str(), buf, 1024);
      return std::string(buf);
    }
    // Get file size
    inline __off_t Size() const { return _st->st_size; }
    // Get file type
    inline bool IsDir() const { return S_ISDIR(_st->st_mode); }
    inline bool IsRegular() const { return S_ISREG(_st->st_mode); }
    inline bool IsPipe() const { return S_ISFIFO(_st->st_mode); }
    inline bool IsLink() const { return S_ISLNK(_st->st_mode); }
    inline bool IsChar() const { return S_ISCHR(_st->st_mode); }
    inline bool IsBlock() const { return S_ISBLK(_st->st_mode); }
    inline bool IsSocket() const { return S_ISSOCK(_st->st_mode); }
    // Get file permission
    inline bool CanUserRead() const { return _st->st_mode & S_IRUSR; }
    inline bool CanUserWrite() const { return _st->st_mode & S_IWUSR; }
    inline bool CanUserExecute() const { return _st->st_mode & S_IXUSR; }
    inline bool CanGroupRead() const { return _st->st_mode & S_IRGRP; }
    inline bool CanGroupWrite() const { return _st->st_mode & S_IWGRP; }
    inline bool CanGroupExecute() const { return _st->st_mode & S_IXGRP; }
    inline bool CanOtherRead() const { return _st->st_mode & S_IROTH; }
    inline bool CanOtherWrite() const { return _st->st_mode & S_IWOTH; }
    inline bool CanOtherExecute() const { return _st->st_mode & S_IXOTH; }
    // Read
    virtual std::string Read(ssize_t size) {
      auto buf = new char[size];
      auto readnum = ::read(_fd, buf, size);
      std::string data(buf);
      delete[] buf;
      return data;
    }
    // Write
    virtual ssize_t Write(const std::string &buf) {
      return ::write(_fd, buf.c_str(), buf.size());
    }
    // Close fd
    int Close() const {
      return ::close(_fd);
    }
  };

  // RegFile is the abstraction of regular file
  class RegFile : public File {
  private:
    std::string _name;
    std::string _path;
    std::string _fullpath;
  public:
    RegFile() = default;
    RegFile(const std::string &path, int flag = O_RDWR | O_CREAT, mode_t mode = 0666)
     : File(::open(path.c_str(), flag, mode)), _path(path) {
      auto buf = new char[1024];
      ::realpath(path.c_str(), buf);
      _fullpath = buf;
      delete[] buf;
      _name = _fullpath.substr(_fullpath.rfind('/') + 1);
    }
    ~RegFile() { Close(); }
    RegFile(RegFile &&regfile) noexcept : File(std::move(regfile)) {
      _path = std::move(regfile._path);
      _name = std::move(regfile._name);
    }
    RegFile& operator=(RegFile &&regfile) {
      static_cast<File&>(*this) = std::move(regfile);
      _path = std::move(regfile._path);
      _name = std::move(regfile._name);
      return *this;
    }
    // Get info
    const std::string& Name() const { return _name; }
    const std::string& Path() const { return _path; }
    // Read
    std::string ReadAll() { return Read(Size()); }
    ssize_t WriteAll(const std::string &buf) {
      ftruncate(fd(), 0);
      return Write(buf);
    }
  };

  // Here are some functions about regular file
  inline RegFile OpenFile(const std::string filepath, int flag = O_RDWR | O_CREAT, mode_t mode = 0666) {
    return RegFile(filepath, flag, mode);
  }
  inline std::string ReadFile(const std::string filepath, int flag = O_RDWR | O_CREAT, mode_t mode = 0666) {
    return RegFile(filepath, flag, mode).ReadAll();
  }
  inline ssize_t WriteFile(const std::string filepath, std::string buf) {
    return RegFile(filepath).WriteAll(buf);
  }

  // PipeFile is the abstraction of pipe
  // PipeFile provide APIs to control pipe read and write
  class PipeFile {
  private:
    File *ReadPipe;
    File *WritePipe;
  public:
    PipeFile() {
      int pfd[2];
      if (::pipe(pfd) == -1)
        err_msg("Pipe creat failed: ret == -1");
      else {
        ReadPipe = new File(pfd[0]);
        WritePipe = new File(pfd[1]);
      }
    }
    ~PipeFile() {
      delete ReadPipe;
      delete WritePipe;
    }
    PipeFile(PipeFile &&pipefile) noexcept {
      ReadPipe = pipefile.ReadPipe;
      WritePipe = pipefile.WritePipe;
      pipefile.ReadPipe = nullptr;
      pipefile.WritePipe = nullptr;
    }
    PipeFile& operator=(PipeFile &&pipefile) {
      ReadPipe = pipefile.ReadPipe;
      WritePipe = pipefile.WritePipe;
      pipefile.ReadPipe = nullptr;
      pipefile.WritePipe = nullptr;
      return *this;
    }
    // I/O
    std::string Input(ssize_t size) {
      return ReadPipe->Read(size);
    }
    ssize_t Output(std::string &buf) {
      return WritePipe->Write(buf);
    }
    // Close Pipe
    int CloseRead() {
      return ReadPipe->Close();
    }
    int CloseWrite() {
      return WritePipe->Close();
    }
  };
  // Directory is the abstraction of directory
  // Directory inherited class Uncopyable to prohibit copy constructor
  // Directory provide APIs for Read files in directory, including traverse and recursive traverse
  class Directory : Uncopyable {
  private:
    std::string _name;
    std::string _path;
    std::string _fullpath;
    DIR *_dir;
  public:
    Directory(const std::string &path, bool create = false, mode_t mode = 0666)
     : _dir(::opendir(path.c_str())), _path(path) {
      auto buf = new char[1024];
      ::realpath(path.c_str(), buf);
      _fullpath = buf;
      delete[] buf;
      _name = _fullpath.substr(_fullpath.rfind('/') + 1);
      if (_dir == nullptr && create == true) {
        ::mkdir(path.c_str(), mode);
        _dir = ::opendir(path.c_str());
      }
      else if (_dir == nullptr)
        err_msg("Open directory failed: ret == nullptr");
    }
    ~Directory() { ::closedir(_dir); }
    Directory(Directory &&directory) {
      _path = std::move(directory._path);
      _name = std::move(directory._name);
      _dir = directory._dir;
      directory._dir = nullptr;
    }
    Directory& operator=(Directory &&directory) {
      _path = std::move(directory._path);
      _name = std::move(directory._name);
      _dir = directory._dir;
      directory._dir = nullptr;
      return *this;
    }
    // Get info
    const std::string& Name() const { return _name; }
    const std::string& Path() const { return _path; }
    // Read dir
    FileInfo Read() const {
      auto file = ::readdir(_dir);
      if (file == nullptr)
        return FileInfo();
      else {
        if (_path == "/")
          return FileInfo("", file->d_name);
        else
          return FileInfo(_path, file->d_name);
      }
    }
    // Traversing
    template<typename Func, typename ...ARGS>
    void Traverse(Func &&func, ARGS &&...args) const {
      while (true) {
        auto file = Read();
        if (file.Name().empty())
          break;
        if (file.Name() == "." || file.Name() == "..")
          continue;
        func(std::move(file), args...);
      }
    }
    template<typename Func, typename ...ARGS>
    void RecursiveTraverse(Func &&func, ARGS &&...args) const {
      while (true) {
        auto file = Read();
        if (file.Name().empty())
          break;
        if (file.IsDir()) {
          if (file.Name() == "." || file.Name() == "..")
            continue;
          Directory child(file.Path());
          child.RecursiveTraverse(func, args...);
        }
        func(std::move(file), args...);
      }
    }
    // Close
    int Close() const {
      return ::closedir(_dir);
    }
  };

  // Here are some functions about directory
  inline Directory OpenDir(const std::string &dirpath, bool create = false, mode_t mode = 0666) {
    return Directory(dirpath, create, mode);
  }
} // namespace pylib

#endif // !PYLIB_FILE