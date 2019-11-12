#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char *filename = "a";

	printf("%d\n%d\n%d\n", access(filename,R_OK), access(filename, W_OK), access(filename, X_OK));
	return 0;
}
