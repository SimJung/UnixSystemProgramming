#include <stdio.h>
#include <unistd.h>

void rm(char* path)
{
	if(access(path, W_OK) == -1)
	{
		char c;
		printf("You don't have write permission. Do you want to remove?(y/n)");
		scanf("%c", &c);
		if(c != 'y' && c !='Y')
			return;
	}
	if(unlink(path) == -1)
	{
		perror("unlink failed ");
		return;
	}
	printf("Unlink succeeded\n");
}

int main(int arge, char** argv)
{
	rm(argv[1]);
	return 0;
}
