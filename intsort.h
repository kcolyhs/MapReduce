#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct IntList{
	int length;
	int capacity;
	char** array;
}intlist;

//Placeholder for Integer List from Sort CSV File
intlist* createIntList(int initlen){
	intlist* newlist = (intlist*)malloc(sizeof(intlist));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist->array = (char**)malloc(initlen*sizeof(char*));
	return newlist;
}

//Placeholder to Dynamically Expand Integer List Array
void expandIntArray(intlist* tlist){
	char** newarr = (char**)malloc(tlist->capacity*2*sizeof(char*));
	memcpy(newarr,tlist->array,tlist->capacity*sizeof(char*));
	tlist->array = newarr;
	tlist->capacity *= 2;
}

//Placeholder to add number to Integer List
void addToIntlist(intlist* tlist,char* token){
	if(tlist->length+1>tlist->capacity){
		expandIntArray(tlist);
	}
	char* newtoken = (char*)malloc(strlen(token)+1);
	strcpy(newtoken,token);
	tlist->array[tlist->length]=newtoken;
	tlist->length++;
	return;
}

//Basic struct to store value of integer in list
typedef struct IntVector{
	int value;
}intvec;

//Dynamic array for storing Integer Vector structs
typedef struct IntList{
	int length;
	int capacity;
	intvec* array;
	pthread_mutex_t lock;
}intvec_list;

//Create New Dynamic Integer Vector List 
intvec_list* createIntVecList(int initlen){
	intvec_list* newlist = (intvec_list*)malloc(sizeof(intvec_list));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist->array = (char**)malloc(initlen*sizeof(intvec));
	return newlist;
}

//Expand Dynamic Array by Doubling Capacity
void expandIntVecArray(intvec_list* vlist){
	intvec* newarr = (intvec*)malloc(vlist->capacity*2*sizeof(intvec));
	memcpy(newarr,vlist->array,vlist->capacity*sizeof(intvec));
	free(vlist->array);
	vlist->array = newarr;
	vlist->capacity *= 2;
}

//Add New Integer from Parser into Dynamic Vector List
void addIntToIntVecList(intvec_list* vlist, int value){
	pthread_mutex_lock(&vlist->lock);
	if(vlist->length+1>vlist->capacity){
		expandVecArray(vlist);
	}
	intvec* newvec = vlist->array+vlist->length;
	newvec->value = value;
	vlist->length++;
	pthread_mutex_unlock(&vlist->lock);
	return;
}

//Parse Integer Sort Inputfile
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