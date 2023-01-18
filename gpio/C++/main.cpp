#include <iostream>
#include <unistd.h>
#include "GPIO.h"

int main(void)
{
	if(geteuid())
	{
		fputs("This program needs to be run as root\n", stderr);
		return 1;
	}
	
	GPIO gp;
	int choice;
	
	gp.setdir("out"); // in or out
	
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
				gp.setval(0);
				break;
			case 1:
				std::cout<<"On\n";
				gp.setval(1);
				break;
			default:
				std::cout<<"Invalid choice\n";
		}
	} while (choice != -1);
	
	return 0;
}
