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

void fatal (char *str) {
	perror(str);
	exit(1);
}

void removeFirst(char *buf){
	if(buf != NULL && *buf == ' '){
		int i=0;
		for(i = 1; buf[i]; i++)
			buf[i-1] = buf[i];
		buf[i-1] = '\0';
	}
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

int pipeProcess(char *t){
	int fd[2];
	pipe(fd);
	
	char temp[BUFSIZ];
	strcpy(temp, t);
	char *t1 = strtok(temp, "|");
	char *t2 = strtok(NULL, "\0");
	char *chk = strchr(t2, '|');
	char* cmdvec[MAX_CMD_ARG];
	
	removeFirst(t1);
	removeFirst(t2);

	switch(fork()){
	case 0:
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		makelist(t1, " \t", cmdvec, MAX_CMD_ARG);
		execvp(cmdvec[0], cmdvec);
	default:
		if(chk != NULL)
			pipeProcess(t2);
		else{
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			makelist(t2, " \t", cmdvec, MAX_CMD_ARG);
			execvp(cmdvec[0], cmdvec);	
		}
	}
}

int main(int argc, char** argv){
	int i = 0;
	int chk_bg = 0;
	int nv;
	char cmdtemp[BUFSIZ], *chkPipe, *cmdin1, *cmdin2, *cmdout1, *cmdout2, *posIn, *posOut;

	signal(SIGCHLD, sigchldHandler);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
	pid_t pid;
	while(1) {
		chkPipe = NULL;
		cmdin1 = NULL;
		cmdin2 = NULL;
		cmdout1 = NULL;
		cmdout2 = NULL;
		posIn = NULL;
		posOut = NULL;

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
		
		posIn = strchr(cmdline, '<');
		posOut = strchr(cmdline, '>');
		
		strcpy(cmdtemp, cmdline);
		
		if(posIn != NULL){
			strtok(cmdline, "<");
			char *cmdin = strtok(NULL, ">");
			if((strchr(cmdin, '|') != NULL)) {
				cmdin1 = strtok(cmdin, "|");
				cmdin2 = strtok(NULL, ">");
			}else{
				cmdin1 = strtok(cmdin, ">");
				cmdin2 = NULL;
			}

			removeFirst(cmdin1);
			removeFirst(cmdin2);
		}

		chkPipe = strchr(cmdline, '|');

		if(posOut != NULL){
			strtok(cmdtemp, ">");
			char *cmdout = strtok(NULL, "<");
			if((strchr(cmdout, '|') != NULL)) {
				cmdout1 = strtok(cmdout, "|");
				cmdout2 = strtok(NULL, "<");
			}else{
				cmdout1 = strtok(cmdout, "<");
				cmdout2 = NULL;
			}

			removeFirst(cmdout1);
			removeFirst(cmdout2);
		}


		if(posIn == NULL && posOut != NULL)
			strcpy(cmdline, cmdtemp);

		if(chkPipe == NULL){
			nv = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
		}
		if(strcmp(cmdvector[0], "exit") == 0)
			return 0;

		if(strcmp(cmdvector[0], "cd") == 0)
			cmd_cd(nv, cmdvector);
		else{
		
			switch(pid = fork()){
			case 0:
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);		
				if(posIn != NULL){
					int fd1 = open(cmdin1, O_RDONLY, 0644);
					dup2(fd1, 0);
					close(fd1);
				}

				if(posOut != NULL){
					int fd2 = open(cmdout1, O_WRONLY|O_CREAT, 0644);
					dup2(fd2, 1);
					close(fd2);
				}

				if(cmdin2 != NULL){
					pipeProcess(cmdin2);
				}

				if(chkPipe == NULL) {
					execvp(cmdvector[0], cmdvector);
				}
				else
					pipeProcess(cmdline);

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

