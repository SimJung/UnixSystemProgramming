#include <unistd.h>
#include <fcntl.h>
int main()
{
	int bytes;
	char buffer[256];
	int fd = open("example.txt",0644);
	do
	{
		bytes = read(fd, buffer, 255);
		if(bytes <= 0) break;
		write(1, buffer, bytes);
	}while(1);
	return 0;
}
