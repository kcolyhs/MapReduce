#include <string.h>
#include<pthread.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
char* toLowerToken(char* token){
        char *letter = token;
        for(; *letter; letter++){
                *letter = tolower((unsigned char) *letter);
        }

        return token;
}

int main(int argc, char *argv[]){
	//mapred –-app [wordcount, sort] –-impl [procs, threads]
	//--maps num_maps –-reduces num_reduces --input infile
	//–-output outfile
	
	int count = 1; 
int afterReduce_fd=shm_open("afterreduce",O_CREAT | O_RDWR, 0666);
                char (*afterReduce)[40];
                ftruncate(afterReduce_fd,(1500*40));
                afterReduce = mmap(0,1500*40, PROT_READ | PROT_WRITE, MAP_SHARED, afterReduce_fd,0);
	for(count=0; count<150; count++){
		printf("%s\n", count);
	}	
}
