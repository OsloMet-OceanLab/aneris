#ifndef PWM_HPP
#define PWM_HPP


#include <string>

namespace pwm
{

enum
{
    GPIO_LOW =      0x1,
    GPIO_HIGH =     0x2,
    GPIO_INPUT =    0x4,
    GPIO_OUTPUT =   0x8
};

class PWM
{
public:
    explicit PWM(int pin = 0);
    virtual ~PWM();
    int setdir(long dir); // Set GPIO Direction (input/output)
    int setval(long val); // Set GPIO Value (high/low)
    int getval(); // Get GPIO Value
    int get_gpionum();
    
private:
    int pwmnum; // GPIO number associated with the instance of an object
};

class PWMError : std::exception
{
public:
    PWMError(const char* message)
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

} // end namespace pwm

#endif
