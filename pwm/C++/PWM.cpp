#include <fstream>
#include <string>
#include <iostream>

#include "PWM.hpp"

namespace pwm
{

PWM::PWM(int pin)
{
    this->pwmnum = pin ? pin : 4; // default is GPIO4
    
    // Open the export file
    std::ofstream ofs("/sys/class/pwm/export", std::ofstream::out);
    if (!ofs.is_open()) throw PWMError("Unable to export PWM");
    // Write the GPIO number to export
    ofs << this->pwmnum;
    ofs.close();
}

PWM::~PWM()
{
    this->setval(0);
    // Open the unexport file
    std::ofstream ofs("/sys/class/pwmnum/unexport", std::ofstream::out);
    if (!ofs.is_open()) throw PWMError("Unable to unexport GPIO");
    // Write GPIO number to unexport
    ofs << this->pwmnum;
    ofs.close();
}

int PWM::setdir(long dir)
{
    // Open the direction file for gpio
    std::string setdir_str = "/sys/class/gpio/gpio" + std::to_string(this->pwmnum) + "/direction";
    std::ofstream ofs(setdir_str.c_str(), std::ofstream::out);
    if (!ofs.is_open()) throw PWMError("Unable to set the direction of GPIO");
    // Write the direction to direction file
    if (dir & GPIO_INPUT) ofs << "in";
    else if (dir & GPIO_OUTPUT) ofs << "out";
    else throw PWMError("Invalid direction chosen");
    ofs.close();
    return 0;
}

int PWM::setval(long val)
{
    // Open the value file for gpio
    std::string setval_str = "/sys/class/gpio/gpio" + std::to_string(this->pwmnum) + "/value";
    std::ofstream ofs(setval_str.c_str());
    if (!ofs.is_open()) throw PWMError("Unable to set the value of GPIO");
    // Write the value to value file
    if (val & GPIO_HIGH) ofs << 1;
    else if (val & GPIO_LOW) ofs << 0;
    ofs.close();
    return 0;
}

int PWM::getval()
{
    // Open the value file for gpio
    std::string getval_str = "/sys/class/gpio/gpio" + std::to_string(this->pwmnum) + "/value";
    std::ifstream ifs(getval_str.c_str());
    if (!ifs.is_open()) throw PWMError("Unable to read the value of GPIO");
    // Read the current gpio value
    int val;
    ifs >> val;
    ifs.close();
    
    if(val) val = 1;
    return val;
}

int PWM::get_pwmnum()
{
    return this->pwmnum;
}

} // end namespace pwm
