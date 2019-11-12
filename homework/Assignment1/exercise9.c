#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NAMELENGTH 4
#define NROOMS 10

char namebuf[NAMELENGTH];
char buf[11][NAMELENGTH];
int offsets[11];
int sizes[11];


int isempty(int);
void showOffset()
{
	int i;
	for(i = 1; i<=NROOMS; i++)
		printf("%d ", offsets[i]);
	printf("\n");
}
void showNames()
{
	int i;
	for(i = 1; i<= NROOMS; i++)
		printf("Room %d : %s\n", i, buf[i]);
}

void updateOffset()
{
	int i;
	for(i=2;i<=NROOMS; i++)
	{
		offsets[i] = offsets[i-1]+sizes[i-1];
	}
}

char *getoccupier(int roomno)
{
	ssize_t nread;
	int infile = -1;
	
	if(infile == -1 && (infile = open("residents", O_RDWR)) == -1)
		return NULL;

	if(roomno > 1)
	{
		offsets[roomno] = offsets[roomno-1] + sizes[roomno-1];
		if(lseek(infile, offsets[roomno], SEEK_SET) == -1)
			return NULL;
	}

	if(nread = read(infile, namebuf, sizes[roomno]) < 0)
		return NULL;

	namebuf[NAMELENGTH-1] = '\0';
	if(namebuf[0] == '\n' || namebuf[0] == '\0')
		sizes[roomno] = 1;
	else
		sizes[roomno] = NAMELENGTH;
	
	if(sizes[roomno] == 1)
		namebuf[0] = '\0';
	return namebuf;
}

int isempty(int roomno)
{
	if(sizes[roomno] == 1)
	{
		return 1;
	}
	return 0;
}

int findfree()
{
	int i;
	for(i=1; i<=NROOMS; i++)
	{
		if(isempty(i))
			return i;
	}
	return -1;
}

int freeroom(int roomno)
{
	off_t offset = offsets[roomno];
	if(sizes[roomno] == 1)
	{
		printf("Room %d is empty.\n", roomno);
		return -1;
	}

	int inf, outf, i;
	inf = open("residents", O_RDONLY);
	outf = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	
	lseek(inf, 0, SEEK_SET);
	for(i=1; i<=NROOMS; i++)
	{
		if(i == roomno)
		{
			sizes[i] = 1;
			lseek(inf, offset+NAMELENGTH, SEEK_SET);
		}
		else if(!isempty(i))
		{
			read(inf, buf[i], NAMELENGTH);
		}
		else
		{
			read(inf, buf[i], 1);
		}
		buf[i][sizes[i]-1] = '\n';
	}
	for(i=1; i<=NROOMS; i++)
	{
		write(outf, buf[i], sizes[i]);
	}

	updateOffset();
	
	close(inf);
	close(outf);
	unlink("residents");
	link("temp", "residents");
	unlink("temp");
	return 0;
}

int addGuest(int roomno, char* name)
{
	if(!isempty(roomno))
	{
		printf("Room %d is not empty.\n", roomno);
		return -1;
	}
	int inf = open("residents", O_RDONLY);
	int outf = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	int i;
	for(i = 1;i<=NROOMS;i++)
	{
		if(i == roomno)
		{
			strcpy(buf[i], name);
			sizes[i] = NAMELENGTH;
			lseek(inf, offsets[i]+1, SEEK_SET);
		}else if(isempty(i))
			read(inf, buf[i], 1);
		else
			read(inf, buf[i], NAMELENGTH);
		buf[i][sizes[i]-1] = '\n';
	}
	for(i=1; i<=NROOMS; i++)
		write(outf, buf[i], sizes[i]);
	
	close(inf);
	close(outf);
	unlink("residents");
	link("temp", "residents");
	unlink("temp");
	updateOffset();
	return 0;
}

int main(int argc, char** argv)
{
	int i, fr, n, room;
	char *p, name[NAMELENGTH];
	for(i=1; i<=NROOMS; i++)
	{
		offsets[i] = (i-1)*NAMELENGTH;
		sizes[i] = NAMELENGTH;
		getoccupier(i);
	}
	updateOffset();
	while(1)
	{
		printf("\n1. Getoccupier\n2. Freeroom\n3. Addguest\n4. Findfree\n5. Exit\n");
		scanf("%d", &n);

		switch(n)
		{
		//Getoccupier
		case 1:
			for(i=1; i<=NROOMS; i++)
			{
				if(p = getoccupier(i))
					printf("Room %2d, %s\n", i, p);
				else if(namebuf[0] == '\n')
					printf("Room %2d is empty.\n", i);
				else
					printf("Error on room %d\n", i);
			}
			break;
		//Freeroom	
		case 2:
			printf("Insert room No : ");
			scanf("%d", &room);
			freeroom(room);
			break;
		//Addguest
		case 3:
			printf("Insert room No : ");
			scanf("%d", &room);
			printf("Insert name : ");
			scanf("%s", name);
			addGuest(room, name);
			break;
		//Findfree
		case 4:
			if((fr = findfree()) >= 0)
				printf("Empty room no : %d\n", fr);
			else
				printf("There is no empty room.\n");
			break;
		//Exit
		case 5:
			return 0;	
		}
	}	
	return 0;
}
