
#include "MapReduce.h"

int main(int argc, char const *argv[])
{
	if(strcmp(argv[1],"wordcount")!=0 && strcmp(argv[1],"sort")!=0){ //if app doesnt equal wordcount AND it doesnt equal sort, the input is wrong
		printf("error, app input not recognized");
		return NULL;
	}
	enum Application app = argv[1]
	if(strcmp(argv[2],"procs")!=0 && strcmp(argv[2],"threads")!=0){
		printf("error, impl input not recognized");
		return NULL;
	}
	enum Implementation imp=argv[2];//should probably use an enum
	int n_maps=atoi(argv[3]);
	if(n_maps<=0){
		printf("error, number of maps is either less than or equal to zero, or is not a number");
		return NULL;
	}
	int n_reduces=atoi(argv[4]);
	if(n_reduces<=0){
		printf("error, number of reduces is either less than or equal to zero, or is not a number");
		return NULL;
	}
	char* infile=argv[5];
	char* outfile=argv[6];

	
	//Map phase
	void* inter_data = map(app,imp,n_maps,infile);//Only return status code
	//Synchronize
	reduce(app,imp,n_reduces,outfile,inter_data);
	return 0;
}
