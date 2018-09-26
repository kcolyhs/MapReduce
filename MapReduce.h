//#include "wordcount.h"
#include "intsort.h"
#include <stdio.h>
#include <stdlib.h>
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
		counter=0;
		for(i=0; i < tokenlist->length; i++){
			addToktoVecList(vecarr[counter],tokenlist->array[i]);
			counter+=1;
			if(counter==n_maps){
				counter=0;
			}
		}
		mergeSort(0,vecarr[0]->length-1,vecarr[0]);
		int test = 0;
		for(test=0; test<vecarr[0]->length-1; test++){
			printf("%s\n",vecarr[0]->array[test].word);

		}
	/*	int test =0;
		for(test=0; test<n_maps; test++){
			int length = vecarr[test]->length;
			int j=0;
			for(j=0; j<length; j++){
				printf("%s\n", vecarr[test]->array[j].word);
			}
			printf("%s\n", "NEXT VECLISTKJSADFLKJASDFJ");
		}*/
	
		//now we need to multithread and mergesort each vecList
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
