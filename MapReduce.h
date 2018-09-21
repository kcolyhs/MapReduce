//#include "wordcount.h"
#include "intsort.h"
#include <stdio.h>
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
	//	wcParseInput() //commented out because we have no parameters in here yet
	}else if(app==sort){
		
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
