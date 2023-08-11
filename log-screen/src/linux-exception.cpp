#include "run-code.h"
#include <errno.h>
#include <cstring>
using namespace SeaDrip::LogScreen;

bool LinuxException::Check(const int ret, LinuxException*(*exception_creator)(void))
{
    if (ret == -1)
    {
        auto exp = exception_creator();
        throw exp;
    }
    return true;
}

LinuxException::LinuxException(const RunCode at_step) : m_rc_step(at_step), m_n_errno(errno), m_s_message(strerror(this->m_n_errno))
{}

const RunCode LinuxException::GetRunCode() const
{
    return this->m_rc_step;
}

const char* LinuxException::what() const throw()
{
    char buff[128] = "";
    sprintf(buff, "[errno: %d]", this->m_n_errno);
    return (std::string(buff) + this->m_s_message).c_str();
}
