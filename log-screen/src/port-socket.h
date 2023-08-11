#ifndef __SD_LOG_SCREEN_SOCK_H__
#define __SD_LOG_SCREEN_SOCK_H__

#include <arpa/inet.h>

namespace SeaDrip::LogScreen
{
    class PortSocket
    {
    public:
        PortSocket();

        const int GetSocketFD() const;

        void ListenPort(const int port) throw();
        const int Accept();
        void Release();
    protected:

    private:
        int m_n_socket_fd;
        struct sockaddr_in m_sock_addr;
        size_t m_n_addr_len;
    };
}

#endif