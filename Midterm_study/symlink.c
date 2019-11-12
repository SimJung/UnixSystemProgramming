#include <unistd.h>

int main(int argc, char **argv)
{
	char pathname[4];
	read(0, pathname, 4);
	pathname[3] = '\0';
	symlink(argv[1], pathname);
}
