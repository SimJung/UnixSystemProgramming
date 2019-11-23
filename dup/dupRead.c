#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void){

	int fd = open("TEST", O_RDONLY, 0644);
	dup2(fd, 0);
	char* s;
	scanf("%s", s);
	if(s == NULL)
		printf("No...\n");
	else
	printf("%s\n", s);
	return 0;
}
