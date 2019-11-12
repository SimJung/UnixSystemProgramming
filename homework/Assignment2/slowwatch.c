#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat sb;

int main(int argc, char** argv)
{
	time_t mtime = 0;
	int ret;
	do
	{
		ret = stat(argv[1], &sb);
		if(sb.st_mtime != mtime)
		{
			mtime = sb.st_mtime;
			printf("mtime : %ld\n", mtime);
		}
	}
	while(ret != -1);
	
	return 0;
}
