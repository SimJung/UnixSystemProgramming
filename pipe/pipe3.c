#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#define BUFSIZE 32
int main() {
	char buffer[BUFSIZE];
	pid_t pid; int fd[2][2];

	assert(pipe(fd[0]) == 0);
	assert(pipe(fd[1]) == 0);

	pid = fork();

	if(pid == 0){
		close(fd[0][1]); close(fd[1][0]);
		read(fd[0][0], buffer, BUFSIZE);
		printf("Child Message : %s\n", buffer);
		write(fd[1][1], buffer, BUFSIZE);
	}
	else if(pid > 0) {
		close(fd[0][0]); close(fd[1][1]);
		write(fd[0][1], "Hello, World!", BUFSIZE);
		read(fd[1][0], buffer, BUFSIZE);
		printf("Parent Message : %s\n", buffer);
	}
	return 0;
}
