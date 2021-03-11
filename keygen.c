#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]){

	srand(time(NULL));
	int keylen = atoi(argv[1]);
	char * KEY = (char*)calloc(keylen, sizeof(char));	

	int i = 0;
	for(; i < keylen; i++){
		KEY[i] = rand()%27 + 65;
		if(KEY[i] == 91){KEY[i] = ' ';} //replace '[' with ' '
	}
	printf("%s\n", KEY); //print key to stdout, which can be redirected to a file

	free(KEY); //free the allocated memory
	return 0;
}
