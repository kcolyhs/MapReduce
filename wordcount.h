#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>


//Dynamic array for storing the tokens after parsing input but before mapping to vectors
//Doubles in size when capacity is reached
//char** array points to an growing array of char* in the heap which in turn point to tokens as null-terminated strings
//static pthread_mutex_t lock;
typedef struct TokenList{
	int length;
	int size;
	int capacity;
	char** array;
}toklist;

//Initiates an empty token list in the heap of length initlen returns a pointer to it
toklist* createTokList(int initlen){
	toklist* newlist = (toklist*)malloc(sizeof(toklist));
	newlist->length = 0;
	newlist->size =0;
	newlist->capacity = initlen;
	newlist->array = (char**)malloc(initlen*sizeof(char*));//changed newlist.array to newlist->array
	return newlist;
}

// Doubles the length of the token list at the address toklist* points to
// by allocating twice the size copying over existing data and then freeing the old array
void expandTokArray(toklist* tlist){
	char** newarr = (char**)malloc(tlist->capacity*2*sizeof(char*));
	memcpy(newarr,tlist->array,tlist->capacity*sizeof(char*));
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
	tlist->size += strlen(newtoken);
	tlist->size += strlen(" ");
	tlist->size += sizeof(int);
	//free(token);
	//printf("%s\n",newtoken);
	tlist->array[tlist->length]=newtoken;
	tlist->length++;
	return;
}

//Basic struct to store key/value pairing of word to its count
typedef struct WordVector{
	char* word;
	int count;
}wordvec;

//Dynamic array for storing vector structs
//Doubles in size when capacity is reached
typedef struct VectorList{
	int length;
	int capacity;
	wordvec* array;
	pthread_mutex_t lock;
}veclist;

// Constructor for VectorList
veclist* createVecList(int initlen){
	veclist* newlist = (veclist*)malloc(sizeof(veclist));
	newlist->length = 0;
	newlist->capacity = initlen;
	newlist->array = (wordvec*)malloc(initlen*sizeof(wordvec));//malloc is casted to char** but newlist->array is a char*. why?
	return newlist;
}

// Doubles the vector arrays capacity
void expandVecArray(veclist* vlist){
	wordvec* newarr = (wordvec*)malloc(vlist->capacity*2*sizeof(wordvec));
	memcpy(newarr,vlist->array,vlist->capacity*sizeof(wordvec));
	free(vlist->array);
	vlist->array = newarr;
	vlist->capacity *= 2;
}

// Adds token to VecList vlist
void addToktoVecList(veclist* vlist,char* token){
	pthread_mutex_lock(&vlist->lock);
	if(vlist->length+1>vlist->capacity){
		expandVecArray(vlist);//expandTokArray takes a toklist, but is give a vlist??? why?? 2ND EDIT, CHANGED EXPANDTOCLIST TO EXPANDVECARRAY
	}
	wordvec* newvec = vlist->array+vlist->length;
	newvec->word = token;
	newvec->count = 1;
	vlist->length++;
	pthread_mutex_unlock(&vlist->lock);
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

// Trims the whitespace and allocates for the new token
char* trimToken(char* token){
	char* start = token;
	while(*start&&isspace(*start)){
		start++;
	}
	if(!*start)
		return NULL;
	char* end = token + strlen(token)-1;
	while(isspace(*end)){
		end--;
	}
	size_t length = end - start +1;
	char* newtoken = malloc(length+1);
	memcpy(newtoken,start,length);
	newtoken[length] = 0;
	return newtoken;
}

void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

//Returns parsed input file
toklist* wcParseInput(char* inputfile){

        FILE* file = fopen(inputfile,"r");
        char buffer[1024];
        char *tok;
	toklist * tokenList = createTokList(100);
        while(fgets(buffer, 1024, file)!=NULL){
                tok = strtok(buffer,"-.,;:!-\r\t\n ");
                while(tok!=NULL){
			//lowercase strtok
			tok = toLowerToken(tok);
			RemoveSpaces(tok);
		//	printf("%s %i\n", tok,strlen(tok));
			//strip white space from token
			if(tok==0)
				continue;
			addToTokenlist(tokenList, tok);
                        tok = strtok(NULL, "-.,;:!- \r\t\n ");
                }
        }

	return tokenList;
}

//splits the vector list between the tasks
void divideVecList(veclist* vlist,int n_reduces){
	int remaining = vlist->length;
	if(n_reduces>remaining){
		return;//Error
	}
	

}

// Struct to hold the meta-data for the individual mapping tasks
typedef struct wordCountMap{
        int s;
        int e;
        toklist * tokenlist;
        veclist* vecarr;
}wordCountMap;

// Constructor for the wordCountMap struct
wordCountMap* createWordCountMap(int start, int end, toklist* tokenlist, veclist* vecarr){
        wordCountMap * count = (wordCountMap*)malloc(sizeof(wordCountMap));
        count->s = start;
        count->e = end;
        count->tokenlist = tokenlist;
        count->vecarr = vecarr;
        return count;
}

void* mapThread(void* arg){
	wordCountMap* tmp = (wordCountMap*)arg;
	int i = tmp->s;
	for(i=tmp->s; i<tmp->e; i++){
		addToktoVecList(tmp->vecarr,tmp->tokenlist->array[i]);
	}
}

// Struct to hold the meta-data for the individual reduce tasks
typedef struct wordCountReduce{
        int s;
        int e;
        veclist* master;
        veclist* vecarr;
}wordCountReduce;

// Constructor for the wordCountReduce struct
wordCountReduce* createWordCountReduce(int start, int end, veclist* master){
	wordCountReduce * count = (wordCountReduce*)malloc(sizeof(wordCountReduce));
	count->s = start;
	count->e = end;
	count->master = master;
	count->vecarr = createVecList(50);
	return count;
}

void* reduceThread(void* arg){
	int count = 0;
	wordCountReduce *tmp = (wordCountReduce*)arg;
	addToktoVecList(tmp->vecarr,tmp->master->array[tmp->s].word);
	int i = tmp->s;
	for(;i<tmp->e-1; i++){
		if(strcmp(tmp->master->array[i].word,tmp->master->array[i+1].word)==0){
			tmp->vecarr->array[count].count ++;// tmp->master->array[i].count+1;
		}else{
			addToktoVecList(tmp->vecarr,tmp->master->array[i+1].word);
			count++;
		}
		
	}
	//pthread_exit((void*) tmp);
	return NULL;
}

void reduceProc(int start, int end,int length){
	int count = 1;
	int where =start;
	int afterReduce_fd=shm_open("afterreduce",O_CREAT | O_RDWR, 0666);
        char (*afterReduce)[40];
        ftruncate(afterReduce_fd,(length*40));
        afterReduce = mmap(0,length*40, PROT_READ | PROT_WRITE, MAP_SHARED, afterReduce_fd,0);
	int i =0;	
	int after_fd;
        after_fd=shm_open("OS", O_CREAT | O_RDWR, 0666);
        char (*after)[30];
        ftruncate(after_fd, length*30);
        after = mmap(0,length*30, PROT_READ | PROT_WRITE, MAP_SHARED, after_fd, 0);
	char *abc = malloc(sizeof(char)*1000);
	char *tmp;
	tmp = malloc(strlen(after[start])+1);
	
	strcpy(tmp,after[start]);
	for(i=start; i<end-1; i++){
		if(strcmp(after[i],after[i+1])==0){
			count+=1;
		//	printf("%s\n", tmp);
		}else{
			tmp=realloc(tmp,strlen(after[i]) + strlen(" \t") + sizeof(char)*5);
			strcat(tmp, " \t");
			sprintf(abc, "%d",count);
			strcat(tmp,abc);
		//	printf("%s\n", tmp);
			strcpy(afterReduce[where],tmp);
			count = 1;
			where+=1;
			tmp = realloc(tmp,strlen(after[i+1])+1);
			strcpy(tmp,after[i+1]);
		}
		
	}
	tmp = realloc(tmp,strlen(after[i])+strlen(" \t") + sizeof(char)*5);
	strcat(tmp, " \t");
	sprintf(abc, "%d", count);
	strcat(tmp,abc);
	strcpy(afterReduce[where],tmp);
	shm_unlink("after");
        shmdt(after);
        shmctl(shmget(after_fd,length*30,O_CREAT | O_RDWR), IPC_RMID, NULL);
}
