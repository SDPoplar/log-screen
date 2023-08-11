#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

void ShowUsage()
{
    std::cout << "usage: tlog content [level]" << std::endl;
    std::cout << "\tlevel: e = error, w = warning, i = info" << std::endl;
}

std::string translate_level(const char ch)
{
    switch (ch)
    {
    case 'e':
        return "error";
    case 'w':
        return "warning";
    default:
        return "info";    
    }
}

void write_string(const int fd, const std::string& content)
{
    int len = content.length();
    write(fd, &len, sizeof(int));
    write(fd, content.c_str(), len);
}

int main(const int argc, char** argv)
{
    if (argc < 2)
    {
        ShowUsage();
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7788);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cout << "connect failed, " << strerror(errno) << std::endl;
        return 1;
    }
    write(fd, "#sdlog  log ", 12);

    write_string(fd, "tester");
    write_string(fd, translate_level((argc > 2) ? argv[2][0] : ' '));
    write_string(fd, std::string(argv[1]));

    close(fd);
    return 0;
}
