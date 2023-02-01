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
	
	GPIO gp(17);
	int choice;
	
	gp.setdir("in"); // in or out
	
	do
	{
		std::cout<<"Enter 1 for read, 0 to exit: ";
		std::cin>>choice;
		std::cout<<std::endl;
		
		switch(choice)
		{
			case 0:
				std::cout<<"Adios\n";
				break;
			case 1:
				std::cout<<gp.getval()<<std::endl;
				break;
			default:
				std::cout<<"Invalid choice\n";
		}
	} while (choice != 0);
	
	return 0;
}
