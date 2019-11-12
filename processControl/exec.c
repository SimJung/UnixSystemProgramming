#include <stdio.h>
#include <unistd.h>
int main(int argc, char **argv) {
	pid_t pid;
	if(argc > 1) {
		pid = fork();
		if(pid == 0) {
			execl(argv[0], argv[0], NULL);
			perror("execl() failed.\n");
		}
		else {
			printf("%s is running as parent.\n", argv[0]);
			return 0;
		}
	}else
		printf("%s is running alone.\n", argv[0]);
	return 0
		;
}
