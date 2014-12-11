//
// Name: command.h
// command.h describes a command struct and methods to manipulate them 
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.0
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //



typedef int (*method)(int argc, char * argv[]);


typedef struct command {
	char **argv;
	int argc;
	unsigned int seqId;
	method run;
} *Command;



Command cmd_create( int argc, char * argv[], unsigned int seqId, method run);


int cmd_exec( Command cmd );


void cmd_destroy(Command cmd);




