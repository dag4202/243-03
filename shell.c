//
// Name: shell.c
// shell.c implements shell management 
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.0
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "shell.h"
#include "command.h"

#define HIST_SIZE 10

static int verboseBool;
static Queue log;
static unsigned int histSize;
extern unsigned int seqNum;



static int shell_opt(int argc, char *argv []){
	int opt;
	int pos_num;
	while ((opt = getopt(argc, argv, "vh:")) != -1) {
		switch(opt) {
		case 'v':
			verboseBool = 1;
			break;
		case 'h':
			if((pos_num =atoi(optarg)) > 0 ){
				histSize = pos_num;
				break;
			}
			fprintf(stderr, "usage: %s [-v] [-h pos_num]\n", argv[0]);
			return INTERNAL_ERROR;
		case '?':
			fprintf(stderr, "usage: %s [-v] [-h pos_num]\n", argv[0]);
			return INTERNAL_ERROR;
		default:
			return SUCCESS;
		}
	}
	return SUCCESS;
}

int shell_init(int argc, char *argv [] ){
	verboseBool = 0;
	histSize = HIST_SIZE;
	switch(shell_opt(argc, argv )){
	case INTERNAL_ERROR:
		return INTERNAL_ERROR;
	case SUCCESS:
		log= que_create();
		seqNum = 1;
		return SUCCESS;
	}
	return SUCCESS;
}


Queue argParser(char *line, unsigned int len){
	unsigned int count = 0;
	char c;
	char *token;
	if(len ==1){
		if(line[0] == '\n'){
			return NULL;
		}
	}
	if (len == 0){
		return NULL;
	}
	if(verboseBool){
		printf("\tcommand: %s\n", line);
	}
	Queue argv = que_create();
	while(count < len ){
		c = line[count];
		if(c == '\''){
			token = strtok(&(line[count]), "\'");
			count+= strlen(token) + 2;
		} else if (c=='\"') {
			token = strtok(&(line[count]), "\"");
			count+= strlen(token) + 2;
		} else if (c==' ' || c=='\t'){
			count++;
			continue;
		} else if (c=='\n'){
			break;
		} else {
			token = strtok(&(line[count]), " 	\n");
			count+= strlen(token) + 1;
			
		}
		que_push( (void *)token, argv);
	}
	if(que_isEmpty(argv)){
		que_destroy(argv);
		return NULL;
	} else {
		return argv;
	}
	
}

int shell_execute( int argc, char * argv[]){
	Command cmd;
	if (argv[0][0] == '!' && (atoi(&(argv[0][1]))!=0)){
		cmd = cmd_create(argc, argv, seqNum, &bang);
	} else if (strcmp(argv[0], "help") == 0){
		cmd = cmd_create(argc, argv, seqNum, &help);
	} else if (strcmp(argv[0], "history") == 0){
		cmd = cmd_create(argc, argv, seqNum, &history);
		
	} else if (strcmp(argv[0], "quit") == 0){
		cmd = cmd_create(argc, argv, seqNum, &quit);
	} else if (strcmp(argv[0], "verbose") == 0 && (argc > 1)){
		cmd = cmd_create(argc, argv, seqNum, &verbose);
	} else {
		cmd = cmd_create(argc, argv, seqNum, &external);
	}
	
	if(verboseBool){
		printf("\tinput command tokens:\n");
		for(int i = 0; i < argc; ++i){
			printf("\t%d: %s\n", i,argv[i]);
		}
	}
	if(seqNum > histSize){
		cmd_destroy((Command)que_pop(log));
	}
	que_push((void *) cmd, log);
	++seqNum;
	return cmd_exec(cmd);
}


int bang( int argc, char * argv [] ){
	assert((argc >= 1) && (argv[0][0] == '!'));
	char *index = &(argv[0][1]); //get the command number
	unsigned int seqId = (unsigned int) atoi(index);
	assert(seqId != 0); 
	unsigned int bottom;
	if( (seqNum)> histSize) {
		bottom = (seqNum - histSize);
	} else {
		bottom = 1;
	}
	if((seqId < (seqNum-1))  && (seqId >= bottom)){
		return cmd_exec((Command)que_get(seqId-bottom, log));
	} 
	fprintf(stderr, "bang: N = \'%d\' out of history's range\n", seqId );
	return INTERNAL_ERROR;
}


int help( int argc, char * argv [] ){
	assert((argc >= 1) && (strcmp(argv[0], "help")==0));
	printf("bang (!N): Re-execute the Nth command in the history list.\n");
	printf("help: Print a list of internal commands.\n");
	printf("history: Print a list of commands executed so far.\n");
	printf("quit: Clean up memory and gracefully terminate the shell.\n");
	printf("verbose (verbose [on|off]): Turn verbose diagnostic mode on or off.\n");
	return SUCCESS;
}


int history( int argc, char * argv [] ){
	assert((argc >= 1) && (strcmp(argv[0], "history")==0));
	Command cmd = (Command )que_front(log);
	int until;
	if (que_size(log) == histSize){
		until = histSize;
	} else {
		until = seqNum - 1;
	}
	for (int i = 0; i < until; ++i){
		cmd = (Command) que_get(i, log);
		printf("%d:", cmd->seqId);
		for (int j = 0; j < cmd->argc; ++j){
			printf(" %s", cmd->argv[j]);
		}
		printf("\n");
	}
	return SUCCESS;
}

void shell_destroy( void ){
	while(!que_isEmpty(log)){
		cmd_destroy((Command )que_pop(log));
	}
	que_destroy( log );
}

int quit( int argc, char * argv [] ){
	assert((argc >= 1) && (strcmp(argv[0], "quit")==0));
	shell_destroy();
	return QUIT; 
}


int verbose( int argc, char * argv [] ){
	assert ((argc >1) && (strcmp(argv[0], "verbose") == 0));
	if ((strcmp(argv[1], "on") == 0)){
		verboseBool = 1;
	} else if (strcmp(argv[1], "off") == 0){
		verboseBool = 0;
	} else {
		fprintf(stderr, "usage: verbose [on|off]\n");
		return INTERNAL_ERROR;
	}
	return SUCCESS;
}

int external(int argc, char * argv[]){
	assert(argc >=1);
	pid_t id, my_id;
	int status;
	
	id = fork();
	switch( id ){
	
	case -1:
		perror("fork");
		return EXTERNAL_ERROR;
	case 0:
		my_id = getpid();
		if(verboseBool){
			printf("\twait for pid %d: %s\n", my_id, argv[0]);
		}
		if (strchr(argv[0], '/')!=NULL){
			if(verboseBool){
				printf("\texecv: %s\n", argv[0]);
			}
			execv(argv[0], argv);
		} else {
			if(verboseBool){
				printf("\texecvp: %s\n", argv[0]);
			}
			execvp(argv[0], argv);
			
		}
		perror(argv[0]);
		_exit(EXIT_FAILURE);
		break;
	default:
		break;
	}
	
	id = wait(&status);
	if (id < 0) {
		perror("wait");
		return EXTERNAL_ERROR;
	} else {
		if(WIFEXITED(status)) {
			printf("command status: %d\n", WEXITSTATUS(status));
		} else if(WIFSIGNALED(status)) {
			printf("interfering signal: %d\n", WTERMSIG(status));
			return EXTERNAL_ERROR;
		} 
	}
	return SUCCESS;
}

