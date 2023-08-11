#include "log-screen.h"
#include "port-socket.h"
#include "epoll-component.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
using namespace SeaDrip::LogScreen;

Listener* Listener::g_p_ins = nullptr;

Listener::Listener() :
    m_comp_config({port: 7788}), m_b_switch(false),
    m_comp_sock(nullptr), m_comp_epoll(nullptr)
{
    Listener::g_p_ins = this;
}

Listener* Listener::GetInstance()
{
    Listener::g_p_ins || new Listener();
    return Listener::g_p_ins;
}

Config& Listener::GetConfig()
{
    return this->m_comp_config;
}

void Listener::Init() throw()
{
    //  prepare port-socket
    this->m_comp_sock = new PortSocket();
    this->m_comp_sock->ListenPort(this->m_comp_config.port);

    //  regist socket fd to epoll
    this->m_comp_epoll = new EpollComponent(500);
    this->m_comp_epoll->RegistEvent(this->m_comp_sock->GetSocketFD());

    //  turn on listener
    this->m_b_switch = true;
}

const RunCode Listener::Run()
{
    while(this->IsRunning())
    {
        this->m_comp_epoll->Wait([](const int fd)->const bool {
            return Listener::GetInstance()->OnSocketCanAccept(fd);
        });
    }

    return RunCode::SUCCESS;
}

const bool Listener::OnSocketCanAccept(const int fd)
{
    if(fd != this->m_comp_sock->GetSocketFD())
    {
        return false;
    }
    int res_fd = this->m_comp_sock->Accept();
    try {
        LinuxException::Check(res_fd, LINUX_EXCEPTION_CREATOR(RunCode::ACCEPT_SOCKET_FD_FAILED));
    } catch(const LinuxException& e) {
        std::cout << e.what() << '\n';
        return true;
    }
    
    return true;
}

void Listener::Release()
{
    this->m_comp_epoll->Release();
    this->m_comp_sock->Release();
}

void Listener::TurnOff()
{
    this->m_b_switch = false;
}

const bool Listener::IsRunning() const
{
    return this->m_b_switch;
}
