#ifndef UTILITY_EPOLLER_H
#define UTILITY_EPOLLER_H

#include <vector>
#include <cstdint>

struct epoll_event;
typedef union epoll_data epoll_data_t;
namespace srlib {
  namespace net {
    static const uint64_t ev_wr_buf = 1;
    static uint64_t ev_rd_buf = 0;
    
    class Epoller {
    private:
      int _epfd;
      std::vector<epoll_event> _event_list;
    public:
      Epoller();
      ~Epoller();
      Epoller(const Epoller &) = delete;
      Epoller(Epoller &&e)noexcept {_epfd = e._epfd, e._epfd = -1;}
      void AddFd(int fd, uint32_t ev, const epoll_data_t &ev_data);
      void DelFd(int fd);
      void ModFd(int fd, uint32_t ev, const epoll_data_t &ev_data);
      int EpollFd() const {return _epfd;}
      std::vector<epoll_event> Wait(int timeout = 0);
    };
  }
}

#endif //UTILITY_EPOLLER_H
