#ifndef GPIO_H
#define GPIO_H

#include <string>

class GPIO
{
public:
    explicit GPIO(int pin = 0);
    virtual ~GPIO();
    int setdir(std::string dir); // Set GPIO Direction ("in" or "out")
    int setval(int val); // Set GPIO Value (on/off)
    int getval(); // Get GPIO Value
    int get_gpionum();
    
private:
    int gpionum; // GPIO number associated with the instance of an object
};

class Error : std::exception
{
public:
    Error(const char* message)
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

#endif
