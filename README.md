# LetMeSeeSee <a href="http://www.kernel.org"><img alt="Platform (GNU/Linux)" src="https://img.shields.io/badge/platform-GNU/Linux-blue.svg"></a> [![Build Status](https://travis-ci.com/zxc479773533/LetMeSeeSee.svg?token=5zDzDfTUA9XfQtccPmCX&branch=master)](https://travis-ci.com/zxc479773533/LetMeSeeSee)

[English](https://github.com/zxc479773533/LetMeSeeSee) | [中文](https://github.com/zxc479773533/LetMeSeeSee/blob/master/README_CN.md)

## About

<div align="center">
  <img src="img/LetMeSeeSee.png">
</div>

**LetMeSeeSee** is an efficient and easy-to-use remote file storage tool designed for `Linux C++` developers and maintainers. It includes a server-side remote data storage library developed by `C++` and a client supporting `Windows/Linux/MacOS` developed by `Java`.

**LetMeSeeSee** is dedicated to solving the problem of server development and maintenance personnel running from the remote monitoring program. Every time data storage and error checking is performed, there is no need to recompile the program, modify the configuration, restart the software, and only need to configure all at the beginning, and the client sends a storage request to the corresponding storage node to obtain remote data.

You can use our library in your server code to launch a remote service with just three lines of code. The data storage function is automatically generated by the macro we provide. See the Usage section for specific instructions. In addition, we also provide a `example`, a remote system monitor developed using the `LetMeSeeSee` library for your reference.

## Installing server library

### Dependencies

We recommend that you use the `CMAKE` tool of `3.10` or higher and `gcc` of version 7.0 and above to get a better use of the build experience. This does not mean that the tool cannot be used in an environment that does not meet the version requirements, but a low-level compilation tool may cause some unexpected errors in some environments.


### Install

Please execute the following commands in order to complete the installation:

```shell
git clone https://github.com/zxc479773533/LetMeSeeSee.git
cd LetMeSeeSee
cd src/server
mkdir build
cd build
cmake ..
make
sudo make install
```

## Usage

### Server

The following is an interface description for the `lmss::Storager` class:

```c++
class Storager {
  public:
    // Startup network service, listen the IP address and port in the parameter
    void ListenAndServe(const std::string &ip, uint16_t port);
    // Scan the source and convert all data storage nodes to json format
    void ScanSourceCode(const std::string &path);
    // Set the log file. If you do not call this function, the default is std::clog. If the parameter is empty, the log will not be printed.
    void SetLogFile(const std::string &filename);
  };
```

An application example developed using `LetMeSeeSee` is as follows:([example/src/RemoteSysMonitor.cpp](example/src/RemoteSysMonitor.cpp))

Some of the segments are selected for explanation here.

```c++
// Include the header file of LetMeSeeSee
#include <LetMeSeeSee/storager.h>

// Store Macro，parameter 1 is the name of storage node, parameter 2 is the file name of storage file
Store(Process_Info, process_info.csv) {
  std::string SaveFile = "../data/process_info.csv"; // Set the path to the data storage file on the server

  // Execute your storage behaviors

  return SaveFile; // Return storage file path
}

int main(int argc, char **argv) {
  // Use Storager
  lmss::Storager storager;

  //Scan the source and convert all data storage nodes to json format
  storager.ScanSourceCode("../src");

  // Start the network service and listen on port 1234
  storager.ListenAndServe("0.0.0.0", 1234);

  pylib::Clock timer;
  while (true) {
  
    // Do your operations
  
  }
}
```

You need to add these codes to your project by following the example above, and you can easily add all the features of `LetMeSeeSee` to your project.

### Example

The sample program is a remote system monitor that you can install by executing the following command:

```shell
cd example
mkdir build
cd build
cmake ..
make
```

When running `RemoteSM`, add different parameters to enable the following functions:

```txt
Usage: ./RemoteSM [Options]
Options:
  --source  <dir> : Specify source directory [Required]
  --datadir <dir> : Specify the savedata directory [Required]"
  --logfile <dir> : Specify the log output directory"
  --local  <freq> : Store data by frequency"
```

### Clinet

We also provide a companion client developed by `Java`, you can download the latest version in [release] (https://github.com/zxc479773533/LetMeSeeSee/releases).

<div align="center">
  <img src="img/client.jpg">
</div>

The client can establish a connection with the server by inputting the `IP address` and `port number` of the server, acquire the data storage node, and request data storage for the corresponding node.

The server can allow up to 20 clients to access at the same time.

## Developers

* [zxcpyp](https://github.com/zxc479773533)
  * Product Manage
  * Server Development
  * UI&Logo Design
* [金修旭](https://github.com/jyxk)
  * Clinet Development
* [Lab-Zjt](https://github.com/Lab-Zjt)
  * Server Development & Maintenance

We will continue to maintain this project and may update or reproduce the client in the future.

Copyright (c) 2018 zxcpyp, 金修旭, Lab-Zjt

Updated at 2019-03-05