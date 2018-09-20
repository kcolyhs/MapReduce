
#include "MapReduce.h"

int main(int argc, char const *argv[])
{
	char* app;
	int imp;//should probably use an enum
	int n_maps;
	int n_reduces;
	char* infile;
	char* outfile;

	
	//Map phase
	void* inter_data = map(app,imp,n_maps,infile);//Only return status code
	//Synchronize
	reduce(app,imp,n_reduces,outfile,inter_data);
	return 0;
}