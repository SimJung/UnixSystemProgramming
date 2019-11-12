#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, char** argv)
{
	umask(0000);
	int i;
	for(i=1; i<argc; ++i)
		mkdir(argv[i], 0777);
	return 0;
}
