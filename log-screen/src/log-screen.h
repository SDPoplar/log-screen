#ifndef __SD_LOG_SCREEN_H__
#define __SD_LOG_SCREEN_H__

#include "run-code.h"

namespace SeaDrip::LogScreen
{
    typedef struct
    {
        int port;
    } Config;

    enum class LogLevel : int
    {
        EMPTY = -1,

        INFO = 0,
        WARNING = 1,
        ERROR = 2,
    };

    class LogItem
    {
    public:
        static LogItem FromLevelString(const std::string& level, const std::string& content);

        LogItem(const LogLevel level, const std::string& content);
        const std::string GetLogContent() const;
        const LogLevel GetLogLevel() const;

        const std::string vendor() const;

    protected:
        LogLevel m_e_level;
        std::string m_s_content;
    };
    
    class Listener
    {
    public:
        static Listener* GetInstance();

        Config& GetConfig();

        void Init() throw();
        const RunCode Run();
        const bool OnSocketCanAccept(const int got_fd);
        void Release();

        const bool IsRunning() const;
        void TurnOff();

    protected:
        Listener();
        const bool OnLog(const int res_fd);
        const bool OnCommand(const int res_fd);

    private:
        static Listener* g_p_ins;

        Config m_comp_config;
        bool m_b_switch;
        class PortSocket* m_comp_sock;
        class EpollComponent* m_comp_epoll;
    };
}

#endif