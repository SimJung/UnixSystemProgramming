#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int lsoct(char* p)
{
	int u=0, g=0, o=0;

	if(p[0] == 'r') u |= 4;	
	if(p[1] == 'w') u |= 2;
	if(p[2] == 'x') u |= 1;
	
	if(p[3] == 'r') g |= 4;	
	if(p[4] == 'w') g |= 2;
	if(p[5] == 'x') g |= 1;
	
	if(p[6] == 'r') o |= 4;	
	if(p[7] == 'w') o |= 2;
	if(p[8] == 'x') o |= 1;

	return (u*8*8 + g*8 + o);
}

int main(int argc, char** argv)
{
	int n = 0;
	if(argv[2][0] >= (int)'0' && argv[2][0] <= (int)'9')
	{
		int i, m = 8*8;
		for(i = 1; i<4; i++)
		{
			n += ((int)argv[2][i] - (int)'0') * m;
			m /= 8;
		}
	}else
		n = lsoct(argv[2]);
	
	if(chmod(argv[1], n) == -1)
		perror("chmod failed ");
	else
		printf("setperm succeed\n");

	return 0;
}
