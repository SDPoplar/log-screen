#include "log-screen.h"
#include <sys/epoll.h>
using namespace SeaDrip::LogScreen;

//  ================================    Listener    ============================

Listener::Listener() : cfg({port: 7788}), m_b_switch(false), m_comp_sock(nullptr), m_n_epoll_fd(epoll_create(100))
{}

Config& Listener::GetConfig()
{
    return this->cfg;
}

const RunCode Listener::Run()
{
    RunCode init_status = this->init();
    if (init_status != RunCode::SUCCESS)
    {
        return init_status;
    }
    else
    {
        this->m_b_switch = true;
    }

    return RunCode::SUCCESS;
}

void Listener::Release()
{}

void Listener::TurnOff()
{
    this->m_b_switch = false;
}

const RunCode Listener::init()
{
    return RunCode::SUCCESS;
}
