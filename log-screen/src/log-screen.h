#ifndef __SD_LOG_SCREEN_H__
#define __SD_LOG_SCREEN_H__

#include "run-code.h"

namespace SeaDrip::LogScreen
{
    typedef struct
    {
        int port;
    } Config;
    
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

    private:
        static Listener* g_p_ins;

        Config m_comp_config;
        bool m_b_switch;
        class PortSocket* m_comp_sock;
        class EpollComponent* m_comp_epoll;
    };
}

#endif