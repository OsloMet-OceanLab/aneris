#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <exception>

namespace Web_Server
{
	
int serve(const int port);

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

} // end namespace Web_Server

#endif
