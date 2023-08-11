#include "log-screen.h"
#include <iostream>

void ShowUsage()
{
    std::cout << "usage: logscreen [-p port]";
}

const bool PickShellOptions(const int argc, char** argv, SeaDrip::LogScreen::Config& config)
{
    return true;
}

int main(const int argc, char** argv)
{
    SeaDrip::LogScreen::Listener engine;
    if(!PickShellOptions(argc, argv, engine.GetConfig()))
    {
        ShowUsage();
        return 0;
    }

    int run_status = static_cast<int>(engine.Run());
    engine.Release();
    return run_status;
}
