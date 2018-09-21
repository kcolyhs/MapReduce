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
	newlist.array = (char**)malloc(initlen*sizeof(char*));
	return newlist;
}
//doubles the length of the token list at the address toklist* points to
//by allocating twice the size copying over existing data and then freeing the old array
void expandTokArray(toklist* tokenlist){
	char** newarr = (char**)malloc(tokenlist->capacity*2*sizeof(char*));
	memcpy(newarr,tokenlist->array,tokenlist->capacity*sizeof(char*));
	tokenlist->array.free();
	tokenlist->array = newarr;
}
//Adds a new entry to the token list
void addToTokenlist(toklist* tokenlist,char* token){
	return;
}


//Returns parsed input file
toklist* wcParseInput(char* inputfile){
	return;
}
/*
Functions to create:
*/

