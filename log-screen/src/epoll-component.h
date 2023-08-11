#ifndef __SD_LOG_SCREEN_EPOLL_H__
#define __SD_LOG_SCREEN_EPOLL_H__

#include <sys/epoll.h>

namespace SeaDrip::LogScreen
{
    class EpollComponent
    {
    public:
        EpollComponent(const int timeout);

        void RegistEvent(const int fd) throw();
        const int Wait(const bool (*event_callback)(const int fd));
        void ConsumeEvent(struct epoll_event* event);
        void Release();

    private:
        int m_n_timeout;
        int m_n_epoll_fd;
        struct epoll_event m_epoll_in_event;
    };
}

#endif