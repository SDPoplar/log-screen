#ifndef __SD_LOG_SCREEN_RUNCODE_H__
#define __SD_LOG_SCREEN_RUNCODE_H__

#include <exception>
#include <string>

namespace SeaDrip::LogScreen
{
    enum class RunCode : int
    {
        SUCCESS                             = 0,

        CREATE_SOCKET_FAILED                = 1,
        BIND_SOCKET_ADDRESS_FAILED          = 2,
        LISTEN_SOCKET_FAILED                = 3,
        REGIST_EPOLL_EVENT_FAILED           = 4,

        ACCEPT_SOCKET_FD_FAILED             = 100,
    };

    class LinuxException : public std::exception
    {
    public:
        static bool Check(const int ret, LinuxException*(*exception_creator)(void));

        LinuxException(const RunCode at_step);
        const RunCode GetRunCode() const;

        const char* what() const throw() override;
    protected:
        RunCode m_rc_step;
        int m_n_errno;
        std::string m_s_message;
    };
}

#define LINUX_EXCEPTION_CREATOR(run_code) \
[]()->SeaDrip::LogScreen::LinuxException* { return new SeaDrip::LogScreen::LinuxException(run_code); }

#endif
