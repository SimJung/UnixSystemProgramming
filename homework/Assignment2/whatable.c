#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	if(access(argv[1], R_OK) == -1)
		perror("Read ");
	else
		printf("Read success\n");

	if(access(argv[1], W_OK) == -1)
		perror("Write ");
	else
		printf("Write success\n");
	
	if(access(argv[1], X_OK) == -1)
		perror("Execute ");
	else
		printf("Execute success\n");

	return 0;
}
