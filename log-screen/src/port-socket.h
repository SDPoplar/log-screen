#ifndef __SD_LOG_SCREEN_SOCK_H__
#define __SD_LOG_SCREEN_SOCK_H__

namespace SeaDrip::LogScreen
{
    class PortSocket
    {
    public:
        PortSocket();

        const int GetSocketFD() const;
    protected:

    private:
        int m_n_socket_fd;
    };
}

#endif