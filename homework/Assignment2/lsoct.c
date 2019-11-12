#include <stdio.h>

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
	printf("%#o\n", lsoct(argv[1]));
	return 0;
}
