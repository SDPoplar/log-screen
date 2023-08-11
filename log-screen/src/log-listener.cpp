#include "log-screen.h"
#include "port-socket.h"
#include "epoll-component.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
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

    char buff[8] = {0};
    int read_bytes = read(res_fd, buff, 8);
    buff[7] = '\0';
    if ((read_bytes < 6) || (boost::to_lower_copy(boost::trim_copy(std::string(buff))) != "#sdlog"))
    {
        std::cout << "got invalid request" << std::endl;
        return true;
    }

    read_bytes = read(res_fd, buff, 4);
    if (read_bytes < 3)
    {
        std::cout << "request broken" << std::endl;
        return true;
    }
    buff[3] = '\0';
    std::string flag = boost::to_lower_copy(boost::trim_copy(std::string(buff)));
    const std::unordered_map<std::string, const bool (Listener::*)(int)> action_map = {
        std::pair<std::string, const bool (Listener::*)(int)>("cmd", &Listener::OnCommand),
        std::pair<std::string, const bool (Listener::*)(int)>("log", &Listener::OnLog),
    };
    auto action = action_map.find(flag);
    if (action == action_map.end())
    {
        std::cout << "unkown action " << flag << std::endl;
        return true;
    }
    auto action_pointer = action->second;
    return (this->*action_pointer)(res_fd);
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

std::string read_sized_string(const int res_fd)
{
    int len;
    if ((read(res_fd, &len, sizeof(int)) <= 0) || (len <= 0) || (len > 10000))
    {
        return "";
    }
    char buff[512] = "";
    std::string ret = "";
    while(len > 0)
    {
        int this_time = (len > 511) ? 511 : len;
        int readed = read(res_fd, buff, this_time);
        if (readed <= 0)
        {
            break;
        }
        buff[readed] = '\0';
        ret += std::string(buff);
        len -= readed;
    };
    boost::trim(ret);
    return ret;
}

const bool Listener::OnCommand(const int res_fd)
{
    return true;
}

const bool Listener::OnLog(const int res_fd)
{
    std::string from_client = read_sized_string(res_fd);
    std::string log_level = boost::to_lower_copy(read_sized_string(res_fd));
    std::cout
        //  << asctime(localtime(time(nullptr)))
        << "[" << from_client << "]"
        << LogItem::FromLevelString(log_level, read_sized_string(res_fd)).vendor()
        << std::endl;
    return true;
}
