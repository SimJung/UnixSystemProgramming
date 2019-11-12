#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1

typedef enum {false, true} bool;

bool isAlpha(char a)
{
	if(((int)a >= 65 && (int)a <= 90) || ((int)a >= 97 && (int)a <= 122))
		return 1;
	return 0;
}

bool isDigit(char a)
{
	if((int)a >= 48 && (int)a <= 57)
		return 1;
	return 0;
}

int main() {

        char buffer[BUFSIZE];
        int filedes;
        ssize_t nread;
        long total = 0, words = 0, lines = 0;
        bool isWord = false;

        if((filedes = open("anotherfile", O_RDONLY)) == -1)
        {
                printf("error in opening anotherfile\n");
                exit(1);
        }

        while((nread = read(filedes, buffer, BUFSIZE)) > 0)
        {
                total += nread;
                if(!isAlpha(buffer[0]) && !isDigit(buffer[0]) && isWord )
                {
			words++;
			isWord = false;
		}else if(isAlpha(buffer[0]) || isDigit(buffer[0]))
			isWord = true;
                if(buffer[0] == '\n')
                        lines++;
	}
	printf("total chars in anotherfile: %ld\n", total);
	printf("total wordsd in anotherfile: %ld\n", words);
	printf("total lines in anotherfile: %ld\n", lines);
	exit(0);
}
