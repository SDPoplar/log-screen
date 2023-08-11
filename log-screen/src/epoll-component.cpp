#include "epoll-component.h"
#include "run-code.h"
#include <unistd.h>
using namespace SeaDrip::LogScreen;

EpollComponent::EpollComponent(const int timeout) : m_n_timeout(timeout), m_n_epoll_fd(epoll_create(100))
{
    this->m_epoll_in_event.events = EPOLLIN;
    this->m_epoll_in_event.data.fd = 0;
}

void EpollComponent::RegistEvent(const int fd) throw()
{
    this->m_epoll_in_event.data.fd = fd;
    LinuxException::Check(
        epoll_ctl(this->m_n_epoll_fd, EPOLL_CTL_ADD, fd, &(this->m_epoll_in_event)),
        LINUX_EXCEPTION_CREATOR(RunCode::REGIST_EPOLL_EVENT_FAILED)
    );
}

const int EpollComponent::Wait(const bool (*event_callback)(const int fd))
{
    struct epoll_event events[64];
    int got_events = epoll_wait(this->m_n_epoll_fd, events, 64, this->m_n_timeout);
    for(int i = 0; i < got_events; i++)
    {
        int fd = events[i].data.fd;
        if (event_callback(fd))
        {
            epoll_ctl(this->m_n_epoll_fd, EPOLL_CTL_MOD, fd, events + i);
        }
    }
    return got_events;
}

void EpollComponent::ConsumeEvent(struct epoll_event* event)
{
    epoll_ctl(this->m_n_epoll_fd, EPOLL_CTL_MOD, event->data.fd, event);
}

void EpollComponent::Release()
{
    epoll_ctl(this->m_n_epoll_fd, EPOLL_CTL_DEL, this->m_epoll_in_event.data.fd, &(this->m_epoll_in_event));
    close(this->m_n_epoll_fd);
}
