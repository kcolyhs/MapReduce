//#include "wordcount.h"
#include "intsort.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include "MergeSort.c"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
enum Implementation{
	procs,
	threads
};

enum Application{
	wordcount,
	sort
};

void* map(enum Application app, enum Implementation imp, int n_maps, char* infile){
	if(app==wordcount && imp==threads){
		toklist * tokenlist = wcParseInput(infile); //tokenlist holds a list of all the words
		veclist * vecarr = createVecList(50);
		int div = tokenlist->length / n_maps;
		int mod = tokenlist->length % n_maps;
		int i =0;
		pthread_t *tid = malloc(n_maps * sizeof(pthread_t));
		for(i=0; i<n_maps; i++){
			int start = i *div;
			int end = (i+1)*div;
			if(i==n_maps-1){
				end+=mod;
			}
			wordCountMap* tmp = createWordCountMap(start,end,tokenlist,vecarr);
			pthread_create(&tid[i],NULL,mapThread,(void*)tmp);
		}

		for(i=0; i<n_maps; i++){
                	pthread_join(tid[i],NULL);
                }

		mergeSort(0,vecarr->length-1,vecarr);


		return (void*)vecarr;
	}else if(app==wordcount && imp == procs){
		toklist * tokenlist = wcParseInput(infile);
                int div = tokenlist->length / n_maps;
                int mod = tokenlist->length % n_maps;
                int i =0;
		int shm_fd;
		int numfd;
		void* ptr;
		int* number;
		pthread_mutex_t *mutex = NULL;
		pthread_mutexattr_t attrmutex;
		pthread_mutexattr_init(&attrmutex);
		pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);
		int des_mutex = shm_open("/mutex_lock", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG);
		ftruncate(des_mutex,sizeof(pthread_mutex_t));
		mutex = (pthread_mutex_t*) mmap(NULL,sizeof(pthread_mutex_t),PROT_READ | PROT_WRITE, MAP_SHARED, des_mutex,0);
		pthread_mutex_init(mutex, &attrmutex);
		shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
		numfd = shm_open("where", O_CREAT | O_RDWR, 0666);
		ftruncate(numfd, sizeof(int));
		ftruncate(shm_fd, tokenlist->length*30);
		number = mmap(0,sizeof(int), PROT_WRITE,MAP_SHARED,numfd,0);
		*number=0;
		//ptr = mmap(0,tokenlist->size,PROT_WRITE,MAP_SHARED,shm_fd,0);

		char (*testing)[30];
		testing =mmap(0,tokenlist->length*30, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
		
		int after_fd;
		after_fd=shm_open("after", O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, tokenlist->length*30);
		char (*after)[30];
		ftruncate(after_fd, tokenlist->length*30);
		after = mmap(0,tokenlist->length*30, PROT_READ | PROT_WRITE, MAP_SHARED, after_fd, 0);
		
		for(;i<n_maps; i++){
			int pid=fork();
			if(pid==0){
				int start = i*div;
				int end = (i+1)*div;
				if(i==n_maps-1)
					end+=mod;
				int j =0;
				pthread_mutex_lock(mutex);
				for(j=start; j<end; j++){
			/*	//	pthread_mutex_lock(mutex);
					ptr +=*number;
					sprintf(ptr, "%s 1", tokenlist->array[j]);
					ptr -=*number;
					
					*number += strlen(tokenlist->array[j]);
					*number += strlen(" 1");
			*/	//	pthread_mutex_unlock(mutex);
			//		printf("%s\n", tokenlist->array[j]);
					strcpy(testing[j], tokenlist->array[j]);
				}
				pthread_mutex_unlock(mutex);
				exit(0);
			}	
		}
		for(i=0; i<n_maps; i++){
			wait(NULL);
		}
/*		veclist * vecarr = createVecList(50);	
		for(i=0; i<tokenlist->length; i++){
			//printf("%s\n", testing[i]);
			addToktoVecList(vecarr, testing[i]);
		}*/
		mergeSortProc(0,tokenlist->length-1, tokenlist->length-1);
		for(i=0; i<tokenlist->length; i++){
			printf("%s\n", after[i]);
		}

/*		mergeSort(0,vecarr->length-1,vecarr);
		for(i=0; i<vecarr->length; i++){
			printf("%s\n", vecarr->array[i]);
		}*/
	}else if(app==sort){
		//intlist * integerlist = intParseInput(infile);
		
	}else{
		return NULL; //ERROR
	}
	/*
	1.Create a process or thread for every one of n_maps
	2.Parse the input file (Parse function special for each app) void* parse(char* inputfile)
	3.Split the data amongst the thr/pro (Initiate the Map for each process)
	4.Collect the combined structure(potentially shuffle) and return a pointer to the shared memory
	*/
}
void reduce(enum Application app, enum Implementation imp,int n_maps, int n_reduces, char* outfile, void* inter_data){
	//Initiate the tasks
	//Split between the tasks
	//Execute the tasks
	//Merge the tasks results
	//Output the merged result
	if(app==wordcount && imp == threads){
		veclist* vecarr = (veclist*)inter_data;
                int j=0;
                for(j=0; j<vecarr->length; j++){
 	               //printf("%s\n", vecarr->array[j].word);
                }
		int div = vecarr->length / n_reduces;
                int mod = vecarr->length % n_reduces;
                int i =0;
                pthread_t *tid = malloc(n_reduces * sizeof(pthread_t));
		wordCountReduce ** reduce = malloc(sizeof(wordCountReduce*)*n_reduces);
		for(j=0; j<n_reduces; j++){
			int start = j *div;
                        int end = (j+1)*div;
                        if(j==n_reduces-1){
                                end+=mod;
                        }
			reduce[j]=createWordCountReduce(start,end,vecarr);
			pthread_create(&tid[j],NULL,reduceThread,(void*)reduce[j]);
		//	printf("%p\n",(void*)reduce[j]->vecarr);
		}
		for(i=0; i<n_reduces; i++){
                	pthread_join(tid[i],NULL);
                }
        	for(i=0; i<n_reduces; i++){//NEED TO MERGE THE END OF ONE LIST AND THE BEGINNING OF THE NEXT LIST, IF SAME
			for(j=0; j<reduce[i]->vecarr->length-1; j++){
				printf("%s,%i\n", reduce[i]->vecarr->array[j].word,reduce[i]->vecarr->array[j].count);
			}
		}	
	}else{
		return NULL;//invalid app
	}
		

}

void* nextTokenList();
