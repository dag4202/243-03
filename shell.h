//
// Name: shell.h
// shell.h an interactive shell's protoype  
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.0
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //




#define INTERNAL_ERROR 0
#define SUCCESS 1
#define EXTERNAL_ERROR -1
#define QUIT 2

#include "queue.h"


unsigned int seqNum;

//Initializes the shell based on initial arguments
//Returns 0 if a usage error occurs and 1 upon success
int shell_init(int argc, char *argv [] );

//Parses a trimmed line into tokens
//Returns a null pointer if line is empty
Queue argParser(char *line, unsigned int len);

int shell_execute( int argc, char *argv [] );

void shell_destroy(void);

int bang( int argc, char * argv [] );


int help( int argc, char * argv [] );


int history( int argc, char * argv [] );


int quit( int argc, char * argv [] );

int verbose( int argc, char * argv [] );

int external( int argc, char *argv [] );

//void log(Command cmd);

