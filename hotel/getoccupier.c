#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "hotel.h"
#include "getoccupier.h"

char buffer[LINELENGTH];

char *getoccupier(int roomno){
	off_t offset;
	ssize_t nread;

	if(listfiledes == -1 && (listfiledes = open(listfile, O_RDONLY))==-1){
		fprintf(stderr, "Error on opening list file \n");
		return NULL;
	}
	offset = (roomno - 1) * LINELENGTH;

	if(lseek(listfiledes, offset, SEEK_SET) == -1){
		fprintf(stderr, "Error on setting offset \n");
		return NULL;
	}
	if(nread = read(listfiledes, buffer, LINELENGTH)<0){
		fprintf(stderr, "Error on reading offset\n");
		return NULL;
	}
	buffer[LINELENGTH-1] = '\0';

	return buffer;
}

void findfree()
{
	
}
