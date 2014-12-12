//
// Name: shell.c
// shell.c implements shell management 
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.1
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

static int verboseBool; //1 if the shell is in verbose mode, 0 otherwise.
static Queue log; // the container for the command log.
static unsigned int histSize; //the size of the command log.
extern unsigned int seqNum; //the current sequence Number.



// shell_opt() handles the verbose option and the history size option
// in a argument list.
static int shell_opt(int argc, char *argv []){
	int opt;
	int pos_num;
	while ((opt = getopt(argc, argv, "vh:")) != -1) {
		switch(opt) {
		case 'v': //verbose option
			verboseBool = 1;
			break;
		case 'h': //history size option
			if((pos_num =atoi(optarg)) > 0 ){
				histSize = pos_num;
				break;
			} // else the number given to -h is inappropriate
			fprintf(stderr, "usage: %s [-v] [-h pos_num]\n", argv[0]);
			return INTERNAL_ERROR;
		case '?': //unknown option
			fprintf(stderr, "usage: %s [-v] [-h pos_num]\n", argv[0]);
			return INTERNAL_ERROR;
		default:
			return SUCCESS;
		}
	}
	return SUCCESS;
}

//Initializes the shell.
int shell_init(int argc, char *argv [] ){
	verboseBool = 0;
	histSize = HIST_SIZE;
	switch(shell_opt(argc, argv )){ //get option values
	case INTERNAL_ERROR:
		return INTERNAL_ERROR; //Usage error
	case SUCCESS:
		log= que_create();
		seqNum = 1;
		return SUCCESS;
	}
	return SUCCESS; //Will not reach
}

//Parses the arguments into tokens stored in a queue.
//Returns null if the line is empty. len must be the size of line
Queue argParser(char *line, unsigned int len){
	assert(strlen(line) == len);
	unsigned int count = 0;
	char c;
	char *token;
	if(len ==1){
		if(line[0] == '\n'){
			return NULL;
		} //newline case - still empty
	}
	if (len == 0){
		return NULL; //empty string
	}
	if(verboseBool){
		printf("\tcommand: %s\n", line); //verbose setting: command: line
	}
	Queue argv = que_create();
	while(count < len ){ 
		c = line[count]; //count is updated when token is found or space
		if(c == '\'' || c=='\"'){ //single quote token found
			token = strtok(&(line[count]), "\'\"");//tokenize
			count+= strlen(token) + 2;
			//increment count (position) by number of chars tokenized
		} else if (c==' ' || c=='\t'){ //whitspace found
			count++; //move it along now, nothing to see here
			continue;
		} else if (c=='\n'){
			break; //Its the end of the line for you...buddy
		} else { //a non-quoted argument is found
			token = strtok(&(line[count]), " 	\n");
			count+= strlen(token) + 1;
			
		}
		que_push( (void *)token, argv); //put the token in the queue
	}
	if(que_isEmpty(argv)){ //no acutal tokens found
		que_destroy(argv);
		return NULL;
	} else {
		return argv;
	}
	
}

//Determines if argv[0] matches the format of an internal command
//and if not, determines that the command is external. shell_execute()
//then creates a command type with appropriate paramaters including the
//matching shell.h matching routine and stores the command in the command
//log.  If the command log is full, the oldest command is destroyed before
//the new command is inserted.  If the verbose mode is on, the tokens are 
//listed in sequence. Sequence number is updated before the command is executed
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

//Attains the string after the exclamation point and determines
//if the string, N, is a number. If it is, the bottom index of the command
//log is determined based on the current sequence Number and its relation
//to the size. Then, if N is within (seqNum -1, bottom], then the command 
// is executed. Reports an error if N is out of range.
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
		Command cmd = (Command) que_get(seqId-bottom, log);
		if (cmd->run == &bang){
			unsigned int N = atoi(&(cmd->argv[0][1]));
			assert(N!=0);
			if ((N == cmd->seqId) || (cmd->seqId == seqId)){
				fprintf(stderr, "bang: infinite command calls\n");
				return INTERNAL_ERROR;
			}
		}
		return cmd_exec(cmd);
	} 
	fprintf(stderr, "bang: N = \'%d\' out of history's range\n", seqId );
	return INTERNAL_ERROR;
}

// Prints a static description of the internal commands.
int help( int argc, char * argv [] ){
	assert((argc >= 1) && (strcmp(argv[0], "help")==0));
	printf("help: Print a list of internal commands.\n");
	printf("bang (!N): Re-execute the Nth command in the history list.\n");
	printf("history: Print a list of commands executed so far.\n");
	printf("quit: Clean up memory and gracefully terminate the shell.\n");
	printf("verbose (verbose [on|off]): Turn verbose diagnostic mode on or off.\n");
	return SUCCESS;
}

//Gets commands from the command log and prints their sequence number
//and arguments in order.
int history( int argc, char * argv [] ){
	assert((argc >= 1) && (strcmp(argv[0], "history")==0));
	Command cmd;
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
//frees the command log per member and then frees the log queue.
void shell_destroy( void ){
	while(!que_isEmpty(log)){
		cmd_destroy((Command )que_pop(log));
	}
	que_destroy( log );
}

//calls shell_destroy and sends a code to the driver to exit gracefully
int quit( int argc, char * argv [] ){
	assert((argc >= 1) && (strcmp(argv[0], "quit")==0));
	shell_destroy();
	return QUIT; 
}

// Determines if the second paramater in argv follows [on|off] and
// changes the verboseBool accordingly. If the second paramater in 
// argv does not follow the usage format, a usage error is sent to 
// stderr and an non-fatal error code is sent to the shell driver.
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

//forks to create a child process to run a command through
//either execv or execvp depending on whether or not the
//first argument in argv has a '/' char in it.  
int external(int argc, char * argv[]){
	assert(argc >=1);
	pid_t id, my_id;
	int status;
	
	id = fork(); //create child
	switch( id ){
	
	case -1: //forking error
		perror("fork");
		return EXTERNAL_ERROR;
	case 0: //child process
		if(verboseBool){
			my_id = getpid(); 
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

