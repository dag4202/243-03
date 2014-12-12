//
// Name: command.h
// command.h describes a command struct and methods to manipulate them 
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.1
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //


/**
* Shorthand
*/
typedef int (*method)(int argc, char * argv[]);

/**
* A command struct to hold commands for a shell.
*/
typedef struct command {
	char **argv;
	int argc;
	unsigned int seqId;
	method run;
} *Command;


/**
* Creates a command type object based on the input aquired from a shell: argv, 
* the sequence number: seqId, and the appropriate shell routine for the command:
* run.
*/
Command cmd_create( int argc, char * argv[], unsigned int seqId, method run);

/**
* Executes the command's associated shell routine.
*/
int cmd_exec( Command cmd );

/**
* Frees all dynamically allocated memory in a command type.
*/
void cmd_destroy(Command cmd);




