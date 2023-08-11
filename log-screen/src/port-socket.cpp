#include "port-socket.h"
#include "run-code.h"
#include <sys/socket.h>
#include <unistd.h>
using namespace SeaDrip::LogScreen;

PortSocket::PortSocket() : m_n_socket_fd(0), m_n_addr_len(sizeof(this->m_sock_addr))
{
    this->m_sock_addr.sin_family = AF_INET;
    this->m_sock_addr.sin_addr.s_addr = INADDR_ANY;

}

const int PortSocket::GetSocketFD() const
{
    return this->m_n_socket_fd;
}

void PortSocket::ListenPort(const int port) throw()
{
    this->m_n_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    LinuxException::Check(this->m_n_socket_fd, LINUX_EXCEPTION_CREATOR(RunCode::CREATE_SOCKET_FAILED));

    this->m_sock_addr.sin_port = htons(port);
    LinuxException::Check(
        bind(this->m_n_socket_fd, (const struct sockaddr*)&(this->m_sock_addr), this->m_n_addr_len),
        LINUX_EXCEPTION_CREATOR(RunCode::BIND_SOCKET_ADDRESS_FAILED)
    );
    LinuxException::Check(listen(this->m_n_socket_fd, 128), LINUX_EXCEPTION_CREATOR(RunCode::LISTEN_SOCKET_FAILED));
}

const int PortSocket::Accept()
{
    socklen_t got_size = this->m_n_addr_len;
    return accept(this->m_n_socket_fd, (sockaddr*)&(this->m_sock_addr), &got_size);
}

void PortSocket::Release()
{
    close(this->m_n_socket_fd);
}
