#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <exception>
#include <string>

namespace Web_Server
{
	
void *serve(ws_t *param);

class WS_Err : std::exception
{
public:
    WS_Err(const char* message)
    {
        this->msg = message;
    }
    virtual const char* what()
    {
        return this->msg;
    }
private:
    const char* msg;
};

struct ws_t
{
    int ws_port;
    std::string ws_mode;
};

} // end namespace Web_Server

#endif
