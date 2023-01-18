#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "GPIO.h"

GPIO::GPIO(int pin)
{
    this->gpionum = pin ? pin : 4; // default is GPIO4
    
    // Open the export file
    std::ofstream ofs("/sys/class/gpio/export", std::ofstream::out);
    if (!ofs.is_open()) throw new ExportError("Unable to export GPIO");
    // Write the GPIO number to export
    ofs << this->gpionum;
    ofs.close();
}

GPIO::~GPIO()
{
    this->setval(0);
    // Open the unexport file
    std::ofstream ofs("/sys/class/gpio/unexport", std::ofstream::out);
    if (!ofs.is_open()) throw new UnexportError("Unable to unexport GPIO");
    // Write GPIO number to unexport
    ofs << this->gpionum;
    ofs.close();
}

int GPIO::setdir(std::string dir)
{
    // Open the direction file for gpio
    std::string setdir_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/direction";
    std::ofstream ofs(setdir_str.c_str(), std::ofstream::out);
    if (!ofs.is_open())
    {
        fprintf(stderr, "Unable to set direction of GPIO %i\n", this->gpionum);
        return -1;
    }
    if (dir != "in" && dir != "out")
    {
        fprintf(stderr, "Invalid direction chosen: %s\n", dir.c_str());
        return -1;
    }
    // Write the direction to direction file
    ofs << dir;
    ofs.close();
    return 0;
}

int GPIO::setval(int val)
{
    // Open the value file for gpio
    std::string setval_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/value";
    std::ofstream ofs(setval_str.c_str());
    if (!ofs.is_open())
    {
        fprintf(stderr, "Unable to set the value of GPIO %i\n", this->gpionum);
        return -1;
    }
    // Write the value to value file
    ofs << val;
    ofs.close();
    return 0;
}

int GPIO::getval()
{
    // Open the value file for gpio
    std::string getval_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/value";
    std::ifstream ifs(getval_str.c_str());
    if (!ifs.is_open())
    {
        fprintf(stderr, "Unable to get value of GPIO %i\n", this->gpionum);
        return -1;
    }
    // Read the current gpio value
    int val;
    ifs >> val;
    ifs.close();
    
    if(!val) val = 1;
    return val;
}

int GPIO::get_gpionum()
{
    return this->gpionum;
}
