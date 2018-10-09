#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct IntList{
	int length;
	int size;
	int capacity;
	int* array;
}intlist;

// Constructor for IntList struct
intlist* createIntList(int initlen){
	intlist* newlist = (intlist*)malloc(sizeof(intlist));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist->array = (int*)malloc(initlen*sizeof(int));
	return newlist;
}

// Doubles the capacity of IntList
void expandIntArray(intlist* list){
	int* newarr = (int*)malloc(list->capacity*2*sizeof(int));
	memcpy(newarr,list->array,list->capacity*sizeof(int));
	//free old array
	list->array = newarr;
	list->capacity *= 2;
}

// Adds a number to IntList
void addToIntlist(intlist* list,int num){
	if(list->length+1>list->capacity){
		expandIntArray(list);
	}
	list->array[list->length]=num;
	list->length++;
	return;
}

// Basic struct to store value of integer in list
// Although for this application it is unnecessary it makes
// adding more info in the future easier
typedef struct IntVector{
	int value;
}intvec;

//Dynamic array for storing Integer Vector structs
typedef struct IntVecList{
	int length;
	int capacity;
	intvec* array;
	pthread_mutex_t lock;
}intvec_list;
  
// Create New Dynamic Integer Vector List 
intvec_list* createIntVecList(int initlen){
	intvec_list* newlist = (intvec_list*)malloc(sizeof(intvec_list));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist->array = (intvec*)malloc(initlen*sizeof(intvec));
	return newlist;
}

// Expand Dynamic Array by Doubling Capacity
void expandIntVecArray(intvec_list* vlist){
	intvec* newarr = (intvec*)malloc(vlist->capacity*2*sizeof(intvec));
	memcpy(newarr,vlist->array,vlist->capacity*sizeof(intvec));
	free(vlist->array);
	vlist->array = newarr;
	vlist->capacity *= 2;
}

// Add New Integer from Parser into Dynamic Vector List vlist
void addIntToIntVecList(intvec_list* vlist, int value){
	pthread_mutex_lock(&vlist->lock);
	if(vlist->length+1>vlist->capacity){
		expandIntVecArray(vlist);
	}
	intvec* newvec = vlist->array+vlist->length;
	newvec->value = value;
	vlist->length++;
	pthread_mutex_unlock(&vlist->lock);
	return;
}

// Parse Integer Sort Inputfile - Returns an intList struct
intlist * intParseInput (char *inputfile){

	FILE *file = fopen(inputfile, "r");
    char buffer[1024];
	intlist * intList = createIntList(100);
	
	while (fgets(buffer, 1024, file) != NULL){
	    char* tok;
	    for (tok = strtok(buffer, "\t"); tok && *tok; tok = strtok(NULL, "\t\n")){
	        addToIntlist(intList, atoi(tok));
	    }
    }

	return intList;
}

// Split Int List among n tasks
void divideIntList(intvec_list* list,int n_reduces){
	int remaining = list->length;
	if(n_reduces>remaining){
		return;
	}
	

}

// Integer Sort Map Struct
typedef struct intSortMap{
        int s;
        int e;
        intlist * integerList;
        intvec_list* intvec_arr;
}intSortMap;

// Start Map for Integer Sort
intSortMap* createIntSortMap(int start, int end, intlist* integerList, intvec_list* intvec_arr){
        intSortMap * count = (intSortMap*)malloc(sizeof(intSortMap));
        count->s = start;
        count->e = end;
        count->integerList = integerList;
        count->intvec_arr = intvec_arr;
        return count;
}

// Map procedure each task must do 
void* mapIntThread(void* arg){
	intSortMap* tmp = (intSortMap*)arg;
	int i = tmp->s;
	for(i=tmp->s; i<tmp->e; i++){
		addIntToIntVecList(tmp->intvec_arr,tmp->integerList->array[i]);
	}
}

// 404 missing method body
void* reduceIntThread(void* arg){

}


