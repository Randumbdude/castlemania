#include "log_dump.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/stat.h>

class TeeBuf : public std::streambuf
{
    std::streambuf *sb1;
    std::streambuf *sb2;

public:
    TeeBuf(std::streambuf *buf1, std::streambuf *buf2)
        : sb1(buf1), sb2(buf2) {}

protected:
    int overflow(int c) override
    {
        if (c == EOF)
            return !EOF;
        if (sb1->sputc(c) == EOF || sb2->sputc(c) == EOF)
            return EOF;
        return c;
    }

    int sync() override
    {
        return sb1->pubsync() == 0 && sb2->pubsync() == 0 ? 0 : -1;
    }
};

#include <direct.h>
void enable_logging()
{
    static std::ofstream logfile;
    static TeeBuf *tee = nullptr;

    // mkdir("logs");
    _mkdir("logs");

    char filename[128];
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);

    std::strftime(filename, sizeof(filename),
                  "logs/log_%Y-%m-%d_%H-%M-%S.txt", tm);

    logfile.open(filename);

    tee = new TeeBuf(std::cout.rdbuf(), logfile.rdbuf());
    std::cout.rdbuf(tee);

    std::cout.setf(std::ios::unitbuf);
}