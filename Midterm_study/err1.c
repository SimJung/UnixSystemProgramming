#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

void main(int argc, char **argv)
{
	int fd;
	if((fd = open("nonesuch", O_RDONLY)) == -1)
		fprintf(stdout, "ENOENT: %s\n", strerror(errno));

	errno = EACCES;
	perror(argv[0]);
}
