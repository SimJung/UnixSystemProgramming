#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	pid_t pid;
	printf("Calling fork()\n");
	pid = fork();
	if(pid > 0)
		printf("Return PID : %d, Parent's PID : %d, Parent's PPID : %d\n", pid, getpid(), getppid());
	else if(pid == 0)
		printf("Child Return PID : %d, Child's PID : %d, Child's PPID : %d\n", pid, getpid(), getppid());
	else
		printf("fork() error.\n");
	return 0;
}
