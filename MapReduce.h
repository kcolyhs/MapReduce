//#include "wordcount.h"
#include "intsort.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "MergeSort.c"
enum Implementation{
	procs,
	threads
};

enum Application{
	wordcount,
	sort
};

void* map(enum Application app, enum Implementation imp, int n_maps, char* infile){
	if(app==wordcount){
		toklist * tokenlist = wcParseInput(infile); //tokenlist holds a list of all the words
		//split the tokenlist and map 
		int i =0;
		int counter =0;
		veclist* vecarr[n_maps];
		while(counter<n_maps){
			vecarr[counter]=createVecList(50);
			counter+=1;
		}
		counter=0;//this loop goes through the tokenlist, and separates them into the n veclists for n maps
		for(i=0; i < tokenlist->length; i++){
			addToktoVecList(vecarr[counter],tokenlist->array[i]);
			counter+=1;
			if(counter==n_maps){
				counter=0;
			}
		}
		//this multithreads the mergesort
		pthread_t *tid = malloc(n_maps * sizeof(pthread_t));
		for(i=0; i<n_maps; i++){
			pthread_create(&tid[i],NULL,mergeThreaded,(void*)vecarr[i]);
		}

		for(i=0; i<n_maps; i++){
			pthread_join(tid[i],NULL);
		}
						


/*		int test =0;
		for(test=0; test<n_maps; test++){
			int length = vecarr[test]->length;
			int j=0;
			for(j=0; j<length; j++){
				printf("%s\n", vecarr[test]->array[j].word);
			}
			printf("%s\n", "NEXT VECLISTKJSADFLKJASDFJ           		NEXT VEC LIST KLAJDFLJASDF");
		}*/
	
		return NULL;
	}else if(app==sort){
		intlist * integerlist = intParseInput(infile);
		
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
void reduce(enum Application app, enum Implementation imp,int n_reduces, char* outfile, void* inter_data){
	/*
	1.
	*/
	

}

void* nextTokenList();
