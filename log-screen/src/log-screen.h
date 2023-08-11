#ifndef __SD_LOG_SCREEN_H__
#define __SD_LOG_SCREEN_H__

#include "run-code.h"
#include "port-socket.h"

namespace SeaDrip::LogScreen
{
    typedef struct
    {
        int port;
    } Config;
    
    class Listener
    {
    public:
        Listener();

        Config& GetConfig();

        const RunCode Run();
        void Release();

        void TurnOff();

    protected:
        const RunCode init();

        Config cfg;

    private:
        bool m_b_switch;
        PortSocket* m_comp_sock;
        int m_n_epoll_fd;
    };
}

#endif