#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	int val = 0, fp = open(argv[1], O_RDWR);
	val = fcntl(atoi(argv[1]), F_GETFL, 0);
	

	printf("%s\n", argv[1]);
	
	printf("%d\n", val & O_ACCMODE);
	switch(val & O_ACCMODE)
	{
		case O_WRONLY : printf("wr\n"); break;
		case O_RDWR :	printf("rw\n"); break;
		case O_RDONLY : printf("re\n"); break;
	}

	val = fcntl(fp, F_GETFL, 0);
	printf("%d\n", val);
	printf("%d\n", val&O_ACCMODE);
	return 0;
}
