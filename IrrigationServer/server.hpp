#ifndef server_hpp__
#define server_hpp__

#include "zone_controller.hpp"

class Server
{
public:
    Server(void);

    void RunServer(void);
    void Terminate(void);

public:
    void PrintfSock(int s, const char* f, ...);
    void PrintfAllSockets(const char* f, ...);

private:
    int ReadFromClient(int filedes);
    int MakeSocket(uint16_t port);

private:
    ZoneController controller_;

    struct Connection
    {
      std::string current_line;
    };

    typedef std::map<int, Connection> Connections;
    Connections connections_;
};

#endif // server_hpp__
