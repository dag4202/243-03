//
// Name: command.c
// command.c is the implementation of a command data type  
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.1
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //

#include <stdlib.h>

#include "command.h"

//Creates a command type object
Command cmd_create( int argc, char * argv[], unsigned int seqId, method run){
	Command cmd = (Command) malloc( sizeof ( struct command ) );
	cmd->argc = argc;
	cmd->argv = argv;
	cmd->run = run;
	cmd->seqId = seqId;
	return cmd;
}

//runs the method in the command cmd, using cmd->argc and cmd-<argv
//as arguments
int cmd_exec( Command cmd ){
	return cmd->run( cmd->argc, cmd->argv );
}

//frees all dynamically allocated memory and assumed that
//the args were dynamically allocated.
void cmd_destroy( Command cmd){
	for(int i = 0; i < cmd->argc; ++i){
		free(cmd->argv[i]);
	}
	free(cmd->argv);
	free(cmd);
}
