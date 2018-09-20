typedef struct WordVector{
	char* word;
	unsigned int count;
}wordvec;
typedef struct VectorList{
	int length;
	wordvec* array;
}veclist;

typedef struct TokenList{
	int length;
	char** array;
	struct TokenList *next;
}toklist;
//Returns parsed input file
toklist wcParseInput(char* inputfile){
	return;
}
/*
Functions to create:
*/

