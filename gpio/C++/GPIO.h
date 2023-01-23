#ifndef GPIO_H
#define GPIO_H

#include <string>

namespace gpio
{

enum
{
    GPIO_LOW =      0x1,
    GPIO_HIGH =     0x2,
    GPIO_INPUT =    0x4,
    GPIO_OUTPUT =   0x8
};

class GPIO
{
public:
    explicit GPIO(int pin = 0);
    virtual ~GPIO();
    int setdir(long dir); // Set GPIO Direction (input/output)
    int setval(long val); // Set GPIO Value (high/low)
    int getval(); // Get GPIO Value
    int get_gpionum();
    
private:
    int gpionum; // GPIO number associated with the instance of an object
};

class GPIOError : std::exception
{
public:
    GPIOError(const char* message)
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

} // end namespace gpio

#endif
