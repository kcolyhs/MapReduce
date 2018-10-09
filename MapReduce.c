#include <string.h>
#include <pthread.h>
#include "MapReduce.h"
#include <stdlib.h>
pthread_t queueThread;

toklist* head = NULL;
toklist* tail = NULL;

pthread_mutex_t mutex;
pthread_mutex_t queueMutex;


int main(int argc, char const *argv[]){
	if(strcmp(argv[1],"wordcount")!=0 && strcmp(argv[1],"sort")!=0){ //if app doesnt equal wordcount AND it doesnt equal sort, the input is wrong
		printf("error, app input not recognized");
		return 0;
	}
	enum Application app;
	if(strcmp(argv[1],"wordcount")==0){
		app = wordcount;	
	}else{
		app=sort;
	}
	if(strcmp(argv[2],"procs")!=0 && strcmp(argv[2],"threads")!=0){
		printf("error, impl input not recognized");
		return 0;
	}
	enum Implementation imp;
	if(strcmp(argv[2],"procs")==0){
		imp = procs;
	}else{
		imp=threads;
	}
	int n_maps=atoi(argv[3]);
	if(n_maps<=0){
		printf("error, number of maps is either less than or equal to zero, or is not a number");
		return 0;
	}
	int n_reduces=atoi(argv[4]);
	if(n_reduces<=0){
		printf("error, number of reduces is either less than or equal to zero, or is not a number");
		return 0;
	}
	char* infile=malloc(strlen(argv[5])+1);
	strcpy(infile,argv[5]);
	char* outfile=malloc(strlen(argv[6])+1);
	strcpy(outfile,argv[6]);

	//Map phase
	void* inter_data = map(app,imp,n_maps,infile,outfile);//Only return status code	

	//Synchronize
	reduce(app,imp,n_maps,n_reduces,outfile,inter_data);
	return 0;
}
