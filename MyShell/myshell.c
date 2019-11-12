#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_CMD_ARG 10

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];

void fatal (char *str) {
	perror(str);
	exit(1);
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST){
	int i = 0;
	int numtokens = 0;
	char *snew = NULL;

	if( (s==NULL) || (delimiters == NULL) ) return -1;

	snew = s+strspn(s, delimiters);
	if ((list[numtokens] = strtok(snew, delimiters)) == NULL)
		return numtokens;

	numtokens = 1;

	while(1){
		if( (list[numtokens] = strtok(NULL, delimiters)) == NULL)
			break;

		if( numtokens == (MAX_LIST-1)) return -1;
		numtokens++;
	}
	return numtokens;
}

int cmd_cd(int argc, char** argv)
{
	if(argc == 1)
		chdir(getenv("HOME"));
	else if(argc == 2) {
		if(chdir(argv[1]))
			fatal("cd");
	}else
		fatal("cd");

	return 1;
}

static void sigchldHandler(int unused){
	int status;
	wait(&status);
}

int main(int argc, char** argv){
	int i = 0;
	int chk_bg = 0;
	int nv;
	
	signal(SIGCHLD, sigchldHandler);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	pid_t pid;
	while(1) {

		fputs(prompt, stdout);
		fgets(cmdline, BUFSIZ, stdin);
		
		if(strlen(cmdline) <= 1)
			continue;

		cmdline[ strlen(cmdline) -1] = '\0';
		
		if(cmdline[strlen(cmdline) -1] == '&') {
			chk_bg = 1;
			cmdline[ strlen(cmdline) -2] = '\0';
		}else
			chk_bg = 0;
		
		nv = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
		
		if(strcmp(cmdvector[0], "exit") == 0)
			return 0;
		
		if(strcmp(cmdvector[0], "cd") == 0)
			cmd_cd(nv, cmdvector);
		else{
		
			switch(pid = fork()){
			case 0:
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				execvp(cmdvector[0], cmdvector);
				fatal("main()");
			case -1:
				fatal("main()");
			default:
				if(!chk_bg)
					wait(NULL);
			}
		}
	}
	return 0;
}

