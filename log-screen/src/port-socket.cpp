#include "port-socket.h"
using namespace SeaDrip::LogScreen;

PortSocket::PortSocket() : m_n_socket_fd(0)
{}

const int PortSocket::GetSocketFD() const
{
    return this->m_n_socket_fd;
}
