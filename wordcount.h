typedef struct WordVector{
	char* word;
	unsigned int count;
}wordvec;
typedef struct VectorList{
	int length;
	int capacity;
	wordvec* array;
}veclist;

typedef struct TokenList{
	int length;
	int capacity;
	char** array;
	struct TokenList *next;
}toklist;

toklist* createTokList(int initlen){
	toklist* newlist = (toklist*)malloc(sizeof(toklist));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist.array = (char**)malloc(initlen*sizeof(char*));
	return newlist;
}
void expandTokArray(toklist*){
	char** newarr = (char**)malloc(newarr->capacity*2*sizeof(char*));
	memcpy(newarr,toklist->array,toklist->capacity*sizeof(char*));
	toklist->array.free();
	toklist->array = newarr;
}
void 


//Returns parsed input file
toklist* wcParseInput(char* inputfile){
	return;
}
/*
Functions to create:
*/

