//#include "wordcount.h"
#include "intsort.h"
#include "MergeSort.c"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
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

//Function prototypes
void* map(enum Application app, enum Implementation imp, int n_maps, char* infile, char* outfile);
void reduce(enum Application app, enum Implementation imp,int n_maps, int n_reduces, char* outfile, void* inter_data);

void* map(enum Application app, enum Implementation imp, int n_maps, char* infile, char* outfile){
	if(app==wordcount && imp==threads){
		toklist * tokenlist = wcParseInput(infile); //tokenlist holds a list of all the words
		veclist * vecarr = createVecList(50);
		int iter = 0;
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
		pthread_mutex_t *mutex = NULL;
		pthread_mutexattr_t attrmutex;
		pthread_mutexattr_init(&attrmutex);
		pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);
		int des_mutex = shm_open("/mutex_lock", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG);
		ftruncate(des_mutex,sizeof(pthread_mutex_t));
		mutex = (pthread_mutex_t*) mmap(NULL,sizeof(pthread_mutex_t),PROT_READ | PROT_WRITE, MAP_SHARED, des_mutex,0);
		pthread_mutex_init(mutex, &attrmutex);
		shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, tokenlist->length*30);

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
				for(j=start; j<end; j++){
					pthread_mutex_lock(mutex);
					strcpy(testing[j], tokenlist->array[j]);
					pthread_mutex_unlock(mutex);
				}
				exit(0);
			}	
		}
		for(i=0; i<n_maps; i++){
			wait(NULL);
		}

		mergeSortProc(0,tokenlist->length, tokenlist->length);
		for(i=0; i<tokenlist->length; i++){
			//			printf("%s\n", testing[i]);
		}
		shmdt(testing);
		//	shm_unlink("OS");
		shm_unlink("after");
		shmctl(shmget(shm_fd,tokenlist->length * 30,O_CREAT | O_RDWR),IPC_RMID,NULL);
		//munmap(testing, tokenlist->length *30);
		shmdt(after);
		shmctl(shmget(after_fd,tokenlist->length*30,O_CREAT | O_RDWR), IPC_RMID, NULL);
		munmap(after, tokenlist->length*30);
		return (void*)tokenlist->length;//??
	}else if(app==sort && imp==threads){
		intlist * integerList = intParseInput(infile);
		intvec_list * intvec_arr = createIntVecList(50);
		int div = integerList->length / n_maps;
		int mod = integerList->length % n_maps;
		int i =0;
		pthread_t *tid = malloc(n_maps * sizeof(pthread_t));
		for(i=0; i<n_maps; i++){
			int start = i *div;
			int end = (i+1)*div;
			if(i==n_maps-1){
				end+=mod;
			}
			intSortMap* tmp = createIntSortMap(start,end,integerList,intvec_arr);
			pthread_create(&tid[i],NULL,mapIntThread,(void*)tmp);
		}

		for(i=0; i<n_maps; i++){
			pthread_join(tid[i],NULL);
		}

		mergeSortInt(0,intvec_arr->length-1,intvec_arr);
		FILE *f = fopen(outfile,"w");
		for(i=0; i<intvec_arr->length; i++){
			fprintf(f, "%i\n", intvec_arr->array[i]);
		}
		fclose(f);
		return (void*)intvec_arr;
	}
	else if(app==sort && imp==procs){
		intlist * intvec_arr = intParseInput(infile);
		int div = intvec_arr->length / n_maps;
		int mod = intvec_arr->length %n_maps;
		int i =0;
		pthread_mutex_t *mutex = NULL;
		pthread_mutexattr_t attrmutex;
		pthread_mutexattr_init(&attrmutex);
		pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);
		int des_mutex = shm_open("/mutex_lock", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG);
		ftruncate(des_mutex,sizeof(pthread_mutex_t));
		mutex = (pthread_mutex_t*) mmap(NULL,sizeof(pthread_mutex_t),PROT_READ | PROT_WRITE, MAP_SHARED, des_mutex,0);
		pthread_mutex_init(mutex, &attrmutex);

		int shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, intvec_arr->length*sizeof(int));

		int *testing;
		testing =mmap(0,intvec_arr->length*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

		int after_fd;
		after_fd=shm_open("after", O_CREAT | O_RDWR, 0666);
		ftruncate(after_fd, intvec_arr->length*sizeof(int));
		int *after;
		after = mmap(0,intvec_arr->length*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, after_fd, 0);


		for(;i<n_maps; i++){
			int pid=fork();
			if(pid==0){
				int start = i*div;
				int end = (i+1)*div;
				if(i==n_maps-1)
					end+=mod;
				int j =0;
				for(j=start; j<end; j++){
					pthread_mutex_lock(mutex);
					//strcpy(testing[j], tokenlist->array[j]);
					testing[j] = intvec_arr->array[j];
					pthread_mutex_unlock(mutex);
				}
				exit(0);
			}
		}
		for(i=0; i<n_maps; i++){
			wait(NULL);
		}

		munmap(after_fd, intvec_arr->length*sizeof(int));

		intvec_list * intvec_arra = createIntVecList(50);

		for(i=0; i<intvec_arr->length; i++){
			addIntToIntVecList(intvec_arra,testing[i]);
		}
		mergeSortInt(0,intvec_arra->length-1,intvec_arra);
		FILE *f = fopen(outfile,"w");
		for(i=0; i<intvec_arra->length; i++){
			fprintf(f, "%i\n", intvec_arra->array[i]);
		}
		fclose(f);
		munmap(testing, intvec_arr->length*sizeof(int));	
	}
}
/*
   1.Create a process or thread for every one of n_maps
   2.Parse the input file (Parse function special for each app) void* parse(char* inputfile)
   3.Split the data amongst the thr/pro (Initiate the Map for each process)
   4.Collect the combined structure(potentially shuffle) and return a pointer to the shared memory
   */
void reduce(enum Application app, enum Implementation imp,int n_maps, int n_reduces, char* outfile, void* inter_data){
	if(app==wordcount && imp == threads){
		veclist* vecarr = (veclist*)inter_data;
		int j=0;
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
		}
		for(i=0; i<n_reduces; i++){
			pthread_join(tid[i],NULL);
		}
		FILE *f = fopen(outfile,"w");
		for(i=0; i<n_reduces; i++){//NEED TO MERGE THE END OF ONE LIST AND THE BEGINNING OF THE NEXT LIST, IF SAME
			for(j=0; j<reduce[i]->vecarr->length-1; j++){
				fprintf(f,"%s\t%i\n", reduce[i]->vecarr->array[j].word,reduce[i]->vecarr->array[j].count);
			}
		}
		fclose(f);
	}else if(app==wordcount && imp == procs){
		int after_fd;
		int length = (int)inter_data;
		after_fd=shm_open("after", O_CREAT | O_RDWR, 0666);
		char (*after)[30];
		ftruncate(after_fd, length*30);
		after = mmap(0,length*30, PROT_READ | PROT_WRITE, MAP_SHARED, after_fd, 0);

		int afterReduce_fd=shm_open("afterreduce",O_CREAT | O_RDWR, 0666);
		char (*afterReduce)[40];
		ftruncate(afterReduce_fd,(length*40));
		afterReduce = mmap(0,length*40, PROT_READ | PROT_WRITE, MAP_SHARED, afterReduce_fd,0);

		char (*testing)[30];
		int shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, (length+1)*30);
		testing =mmap(0,(length+1)*30, PROT_READ | PROT_WRITE, MAP_SHARED , shm_fd, 0);
		int i=0;

		int j=0;
		int div = length / n_reduces;
		int mod = length % n_reduces;

		for(;i<n_reduces; i++){
			int pid=fork();
			if(pid==0){
				int start = i*div;
				int end = (i+1)*div;
				if(i==n_maps-1)
					end+=mod;
				int j =0;

				reduceProc(start,end,length);

				exit(0);
			}
		}
		for(i=0; i<n_reduces; i++){
			wait(NULL);
		}
		int newlen =0;
		for(i=0; i<length; i++){
			if(strcmp(afterReduce[i],"\0")==0){
				continue;
				newlen=i;
				break;
			}
			newlen+=1;
		}

		FILE *f = fopen(outfile, "w");
		for(i=0; i<length; i++){
			if(strcmp(afterReduce[i],"\0")==0)
				continue;
			fprintf(f,"%s\n", afterReduce[i]);
		}
		fclose(f);
		shmdt(afterReduce);	
		shmctl(shmget(afterReduce_fd,length*40,O_CREAT | O_RDWR), IPC_RMID, NULL);
		munmap(afterReduce, length*40);
		shmdt(after);
		shmctl(shmget(after_fd,length*30,O_CREAT | O_RDWR), IPC_RMID, NULL);	
		munmap(after, length*30);
		shm_unlink("after");
		shm_unlink("afterreduce");
		shm_unlink("OS");
	}else if(app==sort && imp == procs){
		int i =0;
		for(;i<n_reduces; i++){
			int pid=fork();
			if(pid==0){

				exit(0);
			}
		}
		for(i=0; i<n_reduces; i++){
			wait(NULL);
		}	

		
	}else if(app==sort && imp == threads){
		int i;
        pthread_t *tid = malloc(n_maps * sizeof(pthread_t));
        for(i=0; i<n_maps; i++){
                //intSortMap* tmp = createIntSortMap(start,end,integerList,intvec_arr);
                pthread_create(&tid[i],NULL,reduceIntThread,(void*)NULL);
        }

        for(i=0; i<n_maps; i++){
                pthread_join(tid[i],NULL);
        }
	} 
}
