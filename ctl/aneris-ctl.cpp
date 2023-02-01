#include <iostream>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <option>\n", argv[0]);
		exit(1);
	}
	
	char inbuf[32];
	int p[2], i;
	
	if (pipe(p) < 0) exit(2);
	
	for (int i = 0; i < argc; ++i) write(p[1], argv[i], 32);
	for (int i = 0; i < argc; ++i)
	{
		read(p[0], inbuf, 32);
		printf("%s\n", inbuf);
	}
	return 0;
}
