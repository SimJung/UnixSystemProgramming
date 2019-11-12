#include <stdio.h>
#include <unistd.h>
#define BUFSIZE 32
int main() {
	int fd[2]; 
	pid_t pid;
	char buf[BUFSIZE];
	pipe(fd);
	pid = fork();
	write(fd[1], "Hello, World!", BUFSIZE);
	if(pid > 0) {
		while(read(fd[0], buf, BUFSIZE) > 0)
			printf("Message : %s\n", buf);
		wait(NULL);
	}
	close(fd[0]); close(fd[1]);
	return 0;
}
