#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE 512

int main(int argc, char** argv) {
	ssize_t nread;
	char buf[SIZE];
	int i;
	if(argc > 1){
		for(i = 1; i<argc; i++){
			int f = open(argv[i], O_RDONLY);
			while((nread = read(f, buf, SIZE)) > 0)
				write(1, buf, nread);
		}
	}else{
		while((nread = read(0, buf, SIZE)) > 0)
			write(1, buf, nread);
	}
	exit(0);
}
