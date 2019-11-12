#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	struct stat sb;
	int n = 0;
	stat(argv[1], &sb);

	printf("Original permission : %#o\n", sb.st_mode & 511);
	if(argv[2][0] == '0')
	{
		n += (argv[2][1] - (int)'0') * 8 * 8;
		n += (argv[2][2] - (int)'0') * 8;
		n += (argv[2][3] - (int)'0');
	}
	else
	{
		n += (argv[2][0] - (int)'0') * 8 * 8;
		n += (argv[2][1] - (int)'0') * 8;
		n += (argv[2][2] - (int)'0');
	}
	sb.st_mode = n;

	if(chmod(argv[1], sb.st_mode) == -1)
	{
		perror("chmod failed ");
		exit(1);
	}

	printf("New permission : %#o\n", sb.st_mode);
	return 0;
	
}
