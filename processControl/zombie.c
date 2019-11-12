#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
	pid_t pid;
	pid = fork();
	if(pid != 0)
		sleep(60);
	else
		printf("Parent becomes zombie for 60 secs.\n");
	return 0;
}

