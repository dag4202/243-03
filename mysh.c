//
// Name: mysh.c 
// mysh.c implements a simple interactive shell
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.3
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //

#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "shell.h"

/**
* The Driver for shell.h
*/
int main (int argc, char *argv[]){
	if (shell_init(argc, argv) == INTERNAL_ERROR){
		exit(EXIT_FAILURE);
	}
	char *buff = NULL;
	size_t buffsize = 0;
	Queue tokens;
	int len;
	int arc;
	char **arv;
	printf("mysh[%d]> ", seqNum);
	while((len = getline(&buff, &buffsize, stdin)) != -1){
		tokens = argParser(buff, len);
		if(tokens==NULL){
			printf("mysh[%d]> ", seqNum);
			continue;
		}
		arc = que_size(tokens);
		arv = malloc(sizeof(char *) * arc);
		char *token;
		for (int i = 0; i < arc; ++i){
			token = (char *)que_pop(tokens);
			arv[i] = malloc(strlen(token) + 1);
			strcpy(arv[i], token);
		}
		que_destroy(tokens);
		int result = shell_execute(arc, arv);
		switch(result){
		case INTERNAL_ERROR:
			break;
		case EXTERNAL_ERROR:
			break;
		case QUIT:
			free(buff);
			exit(EXIT_SUCCESS);
			break; //will never reach this statement
		case SUCCESS:
			break;
		}
		printf("mysh[%d]> ", seqNum);
	}
	free(buff);
	shell_destroy();
	int errbuf = errno;
	if (errbuf == EIO){
		perror("input");
		exit(EXIT_FAILURE);
	}
	printf("\n");
	exit(EXIT_SUCCESS);

}
