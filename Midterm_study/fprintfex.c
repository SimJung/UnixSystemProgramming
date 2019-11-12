#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp = fopen("a", "r+");
	fprintf(stdout, "Standard out(1)\n");
	fprintf(fp, "file a accessed\n");
	return 0;
}
