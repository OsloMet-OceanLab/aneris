#include <iostream>
#include <unistd.h>
#include "GPIO.hpp"

using namespace gpio;

int main(int argc, char** argv)
{
	if(geteuid())
	{
		fputs("This program needs to be run as root\n", stderr);
		return 1;
	}
	
	if(argc != 4)
	{
		fprintf(stderr, "Usage: %s <gpioNum> <gpioDir> <0/1>\n", argv[0]);
		return 2;
	}
	
	GPIO gp(atoi(argv[1]));
	gp.setdir(argv[2]);

	switch(atoi(argv[3]))
	{
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
	
	return 0;
}
