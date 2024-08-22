#ifndef GPIO_HPP
#define GPIO_HPP

#include <exception>

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
    explicit GPIO(int pin = 4, long dir = GPIO_INPUT);
    virtual ~GPIO() noexcept(false); // N.B. allowing the dtor to throw exceptions is not recommended, but should be fine as they're handled
    void setval(long val); // Set GPIO Value (high/low)
    int getval(); // Get GPIO Value
    int get_gpionum();
    long get_direction();
    
private:
    int gpionum; // GPIO number associated with the instance of an object
    long direction;
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
