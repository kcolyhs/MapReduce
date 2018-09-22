#include <string.h>

//Dynamic array for storing the tokens after parsing input but before mapping to vectors
//Doubles in size when capacity is reached
//char** array points to an growing array of char* in the heap which in turn point to tokens as null-terminated strings
typedef struct TokenList{
	int length;
	int capacity;
	char** array;
}toklist;

//Initiates an empty token list in the heap of length initlen returns a pointer to it
toklist* createTokList(int initlen){
	toklist* newlist = (toklist*)malloc(sizeof(toklist));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist->array = (char**)malloc(initlen*sizeof(char*));//changed newlist.array to newlist->array
	return newlist;
}
//doubles the length of the token list at the address toklist* points to
//by allocating twice the size copying over existing data and then freeing the old array
void expandTokArray(toklist* tlist){
	char** newarr = (char**)malloc(tlist->capacity*2*sizeof(char*));
	memcpy(newarr,tlist->array,tlist->capacity*sizeof(char*));
	free(tlist->array);//changed tlist->array.free() to free(tlist->array)
	tlist->array = newarr;
	tlist->capacity *= 2;
}
//Adds a new entry to the token list
void addToTokenlist(toklist* tlist,char* token){
	if(tlist->length+1>tlist->capacity){
		expandTokArray(tlist);
	}
	char* newtoken = (char*)malloc(strlen(token)+1);
	strcpy(newtoken,token);
	free(token);
	tlist->array[length]=newtoken;
	tlist->length++;
	return;
}
//Basic struct to store key/value pairing of word to its count
typedef struct WordVector{
	char* word;
	unsigned int count;
}wordvec;

//Dynamic array for storing vector structs
//Doubles in size when capacity is reached
typedef struct VectorList{
	int length;
	int capacity;
	wordvec* array;
}veclist;

veclist* createVecList(int initlen){
	veclist* newlist = (toklist*)malloc(sizeof(veclist));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist->array = (char**)malloc(initlen*sizeof(wordvec));
	return newlist;
}

void expandVecArray(veclist* vlist){
	wordvec* newarr = (wordvec*)malloc(vlist->capacity*2*sizeof(wordvec));
	memcpy(newarr,vlist->array,vlist->capacity*sizeof(wordvec));
	free(vlist->array);
	vlist->array = newarr;
	vlist->capacity *= 2;
}

void addToktoVecList(veclist* vlist,char* token){
	if(vlist->length+1>vlist->capacity){
		expandTokArray(vlist);
	}
	wordvec* newvec = vlist->array+length;
	newvec->word = token;
	newvec->count = 1;
	vlist->length++;
	return;
}


//Converts all characters in token to lowercase
char* toLowerToken(char* token){
	char *letter = token;
	for(; *letter; letter++){
		*letter = tolower((unsigned char) *letter);
	}
	
	return token;
}

char* trimToken(char* token){
	char* start = token;
	while(*start||isspace(*start)){
		start++;
	}
	if(!*start)
		return null;
	char* end = token + strlen(token)-1;
	while(isspace(*end)){
		end--;
	}
	newtoken = malloc(end-start+2);
	memcpy(newtoken,start);
	*newtoken+end-start+1 = NULL;
} 

//Returns parsed input file
toklist* wcParseInput(char* inputfile){
	FILE *file = fopen(inputfile,"r");
	if(file){
		int c;
		while((c=getc(file)) != EOF){
			
		}
	}else{
		printf("sorry, that file is invalid");
	}



	return;
}
/*
Functions to create:
*/

