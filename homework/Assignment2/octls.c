#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char c[] = "---------";

char* octls(int p)
{
	int n = 0, i, m=1;
	for(i=0; i<3; i++)
	{
		n += (p % 10) * m;
		p /= 10;
		m *= 8;
	}

	if(n & S_IRUSR) c[0] = 'r';	
	if(n & S_IWUSR) c[1] = 'w';	
	if(n & S_IXUSR) c[2] = 'x';	

	if(n & S_IRGRP) c[3] = 'r';	
	if(n & S_IWGRP) c[4] = 'w';	
	if(n & S_IXGRP) c[5] = 'x';	
	
	if(n & S_IROTH) c[6] = 'r';	
	if(n & S_IWOTH) c[7] = 'w';	
	if(n & S_IXOTH) c[8] = 'x';
	return c;
}

int main(int argc, char** argv)
{

	printf("%s\n", octls(atoi(argv[1])));
	return 0;
}
