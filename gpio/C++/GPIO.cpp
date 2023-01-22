#include <fstream>
#include <string>
#include <iostream>

#include "GPIO.h"

namespace gpio
{

GPIO::GPIO(int pin)
{
    this->gpionum = pin ? pin : 4; // default is GPIO4
    
    // Open the export file
    std::ofstream ofs("/sys/class/gpio/export", std::ofstream::out);
    if (!ofs.is_open()) throw GPIOError("Unable to export GPIO");
    // Write the GPIO number to export
    ofs << this->gpionum;
    ofs.close();
}

GPIO::~GPIO()
{
    this->setval(0);
    // Open the unexport file
    std::ofstream ofs("/sys/class/gpio/unexport", std::ofstream::out);
    if (!ofs.is_open()) throw GPIOError("Unable to unexport GPIO");
    // Write GPIO number to unexport
    ofs << this->gpionum;
    ofs.close();
}

int GPIO::setdir(long dir)
{
    // Open the direction file for gpio
    std::string setdir_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/direction";
    std::ofstream ofs(setdir_str.c_str(), std::ofstream::out);
    if (!ofs.is_open()) throw GPIOError("Unable to set the direction of GPIO");
    // Write the direction to direction file
    if (dir & GPIO_INPUT) ofs << "in";
    else if (dir & GPIO_INPUT) ofs << "out";
    else throw GPIOError("Invalid direction chosen");
    ofs.close();
    return 0;
}

int GPIO::setval(long val)
{
    // Open the value file for gpio
    std::string setval_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/value";
    std::ofstream ofs(setval_str.c_str());
    if (!ofs.is_open()) throw GPIOError("Unable to set the value of GPIO");
    // Write the value to value file
    if (dir & GPIO_HIGH) ofs << 1;
    else if (dir & GPIO_LOW) ofs << 0;
    ofs.close();
    return 0;
}

int GPIO::getval()
{
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

} // end namespace gpio
