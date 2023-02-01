#include <iostream>
#include <unistd.h>
#include "GPIO.hpp"

using namespace gpio;

int main(void)
{
	if(geteuid())
	{
		fputs("This program needs to be run as root\n", stderr);
		return 1;
	}
	try
	{
	GPIO gp;
	int choice;
	
	gp.setdir(GPIO_OUTPUT); // in or out
	
	do
	{
		std::cout<<"Enter 1 for on, 0 for off, -1 to exit: ";
		std::cin>>choice;
		std::cout<<std::endl;
		
		switch(choice)
		{
			case -1:
				std::cout<<"Adios\n";
				break;
			case 0:
				std::cout<<"Off\n";
				gp.setval(GPIO_LOW);
				break;
			case 1:
				std::cout<<"On\n";
				gp.setval(GPIO_HIGH);
				break;
			default:
				std::cout<<"Invalid choice\n";
		}
	} while (choice != -1);
	}
	catch (GPIOError& e)
	{
		std::cout<<e.what()<<std::endl;
	}
	return 0;
}
