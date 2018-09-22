#include <string.h>
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
	tlist->array[length]=newtoken;
	tlist->length++;
	return;
}
veclist* createVecList(int initlen){
	veclist* newlist = (toklist*)malloc(sizeof(wordvec));
}

//Converts all characters in token to lowercase
char* toLowerToken(char* token){
	char *letter = token;
	for(; *letter; letter++){
		*letter = tolower((unsigned char) *letter);
	}
	
	return token;
}

//Returns parsed input file
toklist* wcParseInput(char* inputfile){

        FILE* file = fopen(inputfile,"r");
        char buffer[1024];
        char *tok;
	toklist * tokenList = createTokList(100);
        while(fgets(buffer, 1024, file)!=NULL){
                tok = strtok(buffer," .,;:!-");
                while(tok!=NULL){
			//lowercase strtok
			tok = toLowerToken(tok);
			//strip white space from token

			addToTokenlist(tokenList, tok)
                        tok = strtok(NULL, " .,;:!-");
                }
        }


	return tokenList;
}
/*
Functions to create:
*/

