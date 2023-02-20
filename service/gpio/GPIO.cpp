#include <fstream>
#include <string>
//#include <iostream>

#include "GPIO.hpp"

namespace gpio
{

GPIO::GPIO(int pin, long dir)
{
    this->gpionum = pin; // default is GPIO4
    this->direction = dir;
    
    // Export the pin
    std::ofstream export_pin("/sys/class/gpio/export", std::ofstream::out);
    if (!export_pin.is_open()) throw GPIOError("Unable to export GPIO");
    // Write the GPIO number to export
    export_pin << this->gpionum;
    export_pin.close();

    // Open the direction file for gpio
    std::string setdir_str = "/sys/class/gpio/gpio" + std::to_string(pin) + "/direction";
    std::ofstream setdir(setdir_str.c_str(), std::ofstream::out);
    if (!setdir.is_open()) throw GPIOError("Unable to set the direction of GPIO");
    // Write the direction to direction file
    if (this->direction & GPIO_INPUT) setdir << "in";
    else if (this->direction & GPIO_OUTPUT) setdir << "out";
    else throw GPIOError("Invalid direction chosen");
    setdir.close();
}

GPIO::~GPIO()
{
    // Unexport the pin
    std::ofstream ofs("/sys/class/gpio/unexport", std::ofstream::out);
    if (!ofs.is_open()) throw GPIOError("Unable to unexport GPIO");
    // Write GPIO number to unexport
    ofs << this->gpionum;
    ofs.close();
}

void GPIO::setval(long val)
{
    if (this->direction & GPIO_INPUT) throw GPIOError("Trying to set an output value on an input");

    // Open the value file for gpio
    std::string setval_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/value";
    std::ofstream ofs(setval_str.c_str());
    if (!ofs.is_open()) throw GPIOError("Unable to set the value of GPIO");
    // Write the value to value file
    if (val & GPIO_HIGH) ofs << 1;
    else if (val & GPIO_LOW) ofs << 0;
    else throw GPIOError("Invalid value chosen");
    ofs.close();
}

int GPIO::getval()
{
    if (this->direction & GPIO_OUTPUT) throw GPIOError("Trying to read an input value on an output");
    
    // Open the value file for gpio
    std::string getval_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/value";
    std::ifstream ifs(getval_str.c_str());
    if (!ifs.is_open()) throw GPIOError("Unable to read the value of GPIO");
    // Read the current gpio value
    int val;
    ifs >> val;
    ifs.close();
    
    if(val) val = 1;
    return val;
}

int GPIO::get_gpionum()
{
    return this->gpionum;
}

long GPIO::get_direction()
{
    return this->direction;
}

} // end namespace gpio
