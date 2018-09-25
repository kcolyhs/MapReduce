#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *trimwhitespace(char *str)
{
  char *end;
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end[1] = '\0';

  return str;
}

int main(int argc, char *argv[]){
	//mapred –-app [wordcount, sort] –-impl [procs, threads]
	//--maps num_maps –-reduces num_reduces --input infile
	//–-output outfile
	
	FILE* file = fopen(argv[1],"r+");
	char buffer[1024];
	char *tok;
	while(fgets(buffer, 1024, file)!=NULL){
		char * duplicate = strdup(buffer);
		tok = strtok(duplicate,"-.,;:!-\n ");
		tok[0]="b";
		while(tok!=NULL){
			printf("%s\n", tok);
			tok = strtok(NULL, "-\n.,;:! ");
		}
	}
}
