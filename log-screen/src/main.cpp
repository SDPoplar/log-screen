#include "log-screen.h"
#include <iostream>
#include <signal.h>

void ShowUsage()
{
    std::cout << "usage: logscreen [-p port]";
}

void TurnOffListener(int sig)
{
    SeaDrip::LogScreen::Listener::GetInstance()->TurnOff();
}

const bool PickShellOptions(const int argc, char** argv, SeaDrip::LogScreen::Config& config)
{
    return true;
}

int main(const int argc, char** argv)
{
    SeaDrip::LogScreen::Listener* engine = SeaDrip::LogScreen::Listener::GetInstance();
    if(!PickShellOptions(argc, argv, engine->GetConfig()))
    {
        ShowUsage();
        return 0;
    }

    try {
        engine->Init();
    } catch(SeaDrip::LogScreen::LinuxException &e)
    {
        std::cout << e.what() << std::endl;
        return static_cast<int>(e.GetRunCode());
    }

    signal(SIGINT, TurnOffListener);
    signal(SIGUSR1, TurnOffListener);
    int run_status = static_cast<int>(engine->Run());
    engine->Release();
    return run_status;
}
