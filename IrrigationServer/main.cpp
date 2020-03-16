
#include <string>
#include <signal.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <time.h>
#include "server.hpp"

Server s;

void TerminationHandler(void)
{
    s.Terminate();
}

void SignalHandler(int signum)
{
    s.Terminate();
}

int main (void)
{
    atexit(TerminationHandler);
    if (signal (SIGINT, SignalHandler) == SIG_IGN)
        signal (SIGINT, SIG_IGN);
    if (signal (SIGHUP, SignalHandler) == SIG_IGN)
        signal (SIGHUP, SIG_IGN);
    if (signal (SIGTERM, SignalHandler) == SIG_IGN)
        signal (SIGTERM, SIG_IGN);

    s.RunServer();

}
