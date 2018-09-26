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