#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_CMD_ARG 10

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char* cmdinvector[MAX_CMD_ARG];
char* cmdoutvector[MAX_CMD_ARG];
char* cmdpipevector[MAX_CMD_ARG];
char cmdline[BUFSIZ];
int pp[2];

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
		if(chdir(argv[1])){
			perror("cd");
			return -1;
		}
			
	}else{
		perror("cd");
		return -1;
	}

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
	int chk_in = 0, chk_out = 0, chk_pipe = 0;
	char cmdtemp[BUFSIZ], *cmdin, *cmdout, *cmdpipe;


	signal(SIGCHLD, sigchldHandler);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
	pipe(pp);
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
		
		chk_in   = (strchr(cmdline, '<') != NULL) ? 1 : 0;
		chk_out  = (strchr(cmdline, '>') != NULL) ? 1 : 0;
		chk_pipe = (strchr(cmdline, '|') != NULL) ? 1 : 0;
		strcpy(cmdtemp, cmdline);
		
		if(chk_in){
			strtok(cmdline, "<");
			cmdin = strtok(NULL, ">");
			makelist(cmdin, " \t", cmdinvector, MAX_CMD_ARG);
		}

		if(chk_out){
			strtok(cmdtemp, ">");
			cmdout = strtok(NULL, "<");
			makelist(cmdout, " \t", cmdoutvector, MAX_CMD_ARG);
		}

		nv = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

		if(strcmp(cmdvector[0], "exit") == 0)
			return 0;
		
		if(strcmp(cmdvector[0], "cd") == 0)
			cmd_cd(nv, cmdvector);
		else{
		
			switch(pid = fork()){
			case 0:
				if(chk_in){
					int fd = open(cmdinvector[0], O_RDONLY, 0644);
					dup2(fd, 0);
					close(fd);
				}

				if(chk_out){	
					int fd = open(cmdoutvector[0], O_WRONLY|O_CREAT, 0644);
					dup2(fd, 1);
					close(fd);
				}
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

