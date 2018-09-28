#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* toLowerToken(char* token){
        char *letter = token;
        for(; *letter; letter++){
                *letter = tolower((unsigned char) *letter);
        }

        return token;
}

int main(int argc, char *argv[]){
	//mapred –-app [wordcount, sort] –-impl [procs, threads]
	//--maps num_maps –-reduces num_reduces --input infile
	//–-output outfile
	
	printf("%i", strcmp("’abolished","which"));
}
