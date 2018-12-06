#include "epoller.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <cstdio>
#include <unistd.h>

namespace srlib {
  namespace net {
    Epoller::Epoller() : _event_list(16) {
      if ((_epfd = epoll_create(1)) < 0)perror("epoll_create");
    }
    Epoller::~Epoller() {
      close(_epfd);
    }
    void Epoller::AddFd(int fd, uint32_t event, const epoll_data_t &ev_data) {
      epoll_event e{event, {ev_data}};
      if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &e) < 0)perror("epoll_ctl");
    }
    void Epoller::DelFd(int fd) {
      if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr) < 0)perror("epoll_ctl");
    }
    void Epoller::ModFd(int fd, uint32_t event, const epoll_data_t &ev_data) {
      epoll_event e{event, {ev_data}};
      if (epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &e) < 0)perror("epoll_ctl");
    }
    std::vector<epoll_event> Epoller::Wait(int timeout) {
      auto ready = epoll_wait(_epfd, _event_list.data(), _event_list.size(), timeout);
      if (ready < 0) {
        perror("epoll_wait");
        return {};
      }
      if (ready >= _event_list.size()) {_event_list.resize(_event_list.size() * 2);}
      std::vector<epoll_event> res(_event_list.begin(), _event_list.begin() + ready);
      return res;
    }
  }
}