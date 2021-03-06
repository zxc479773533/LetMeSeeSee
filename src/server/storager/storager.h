#ifndef LETMESEESEE_STORE_H
#define LETMESEESEE_STORE_H

#include <unordered_map>
#include <vector>

namespace lmss {
  // 单例模式
  // With C++11, static variable's construction is thread-safety.
  template<typename T>
  class Singleton {
  public:
    static T &GetInstance() {
      static T ins{};
      return ins;
    }
  };
  
  // 数据储存节点函数类型。
  typedef std::string(*store_t)();
  // 假反射模板类。
  template<typename T> using Reflector = std::unordered_map<std::string, T>;
  
  class Storager {
  private:
    // 使用单例模式避免注册函数在Reflector初始化前被调用。
    typedef Singleton<Reflector<store_t>> reflector;
  private:
    // json格式的节点列表。
    std::vector<std::string> _node_list;
    std::string _password;
    
  public:
    // 注册函数，将一个函数注册到Reflector中。一般来说只会通过Store宏被调用。
    static void Register(const std::string &name, store_t func) {reflector::GetInstance()[name] = func;}
    // 通过函数名调用函数，返回数据保存的文件的位置。
    static std::string Call(const std::string &name);
    void ListenAndServe(const std::string &ip, uint16_t port);
    // 扫描源码，并将所有数据储存节点转换为json格式保存。
    void ScanSourceCode(const std::string &path);
    // 设置密码，如果不设置或者设置为空，则允许任何用户连接。
    void SetPassword(const std::string& password);
    // 返回节点列表。
    std::vector<std::string> &GetNodeList() {return _node_list;}
    // 设置日志文件，默认为std::clog，如果为空则不打印日志。
    void SetLogFile(const std::string &filename);
  };
// Store宏，第一个参数是数据储存节点的名称，之后的参数为该节点储存的数据的名称。
// 通过helper变量调用Storager的注册函数。
// Usage:
// Store(Node1,Data1,Data2){
//   do_something();
//   return file_name;
// }
#define Store(name, ...) std::string name();\
static auto name##helper = (lmss::Storager::Register(#name,name),0);\
std::string name()
}

#endif
