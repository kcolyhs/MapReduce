#include <string.h>
#include <pthread.h>
#include "MapReduce.h"

pthread_t queueThread;

toklist* head = NULL;
toklist* tail = NULL;

pthread_mutex_t mutex;
pthread_mutex_t queueMutex;


int main(int argc, char const *argv[])
{
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
	enum Implementation imp;//should probably use an enum
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
	char* infile;
	strcpy(infile,argv[5]);
	char* outfile;
	strcpy(outfile,argv[6]);

	
	//Map phase
	void* inter_data = map(app,imp,n_maps,infile);//Only return status code
	

	//Synchronize
	reduce(app,imp,n_reduces,outfile,inter_data);
	return 0;
}

void* map(enum Application app, enum Implementation imp, int n_maps, char* infile){


	//Program each task to map a token list to a vector list and combine them after each task finishes
	if(imp == threads){
		pthread_mutex_init(&mutex,NULL);

		pthread_create(&queueThread,0, &nextTokenList, "");
		pthread_detach(queueThread);

	}
	else if (imp==procs){
		//Next TODO
	}


}

void* getTokenListHead(){
	if(head==NULL){
		return NULL;
	}
	return head->array;
}

void removeTokenListHead(){
	toklist* record = head;
	if(head==NULL){
		printf("Queue is Empty!");
	}
	if(head==tail){
		head=tail=NULL;
	}
	else{
		head=head->next;
	}
	free(record);
}

int queueLength(){
	toklist* record;
	record = head;
	int count = 0;
	if(head == NULL && tail == NULL){
		return count;
	}
	while(record){
		record = record->next;
		count++; 
	}
	return count;
}

void* nextTokenList(){
	while(1)
	{
		pthread_mutex_lock(&queueMutex);
		int count = queueLength();
		pthread_mutex_unlock(&queueMutex);
		if(count>0){
			pthread_mutex_lock(&queueMutex);
			head = getTokenListHead();
			removeTokenListHead();
			pthread_mutex_unlock(&queueMutex);
			//Insert Method to Combine to VectorList
		}
	}
}
