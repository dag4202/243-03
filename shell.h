//
// Name: shell.h
// shell.h an interactive shell's protoype  
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.1
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //




#define INTERNAL_ERROR 0
#define SUCCESS 1
#define EXTERNAL_ERROR -1
#define QUIT 2

#include "queue.h"


unsigned int seqNum;

/**
* Initializes the shell based on first arguments passed
* to shell. Reads in options [-v] and [-h pos_num] and 
* changes internal data accordingly
* Returns a code defined above.
*/
int shell_init(int argc, char *argv [] );

/**
* Parses a line into tokens managable to the driver. Returns NULL
* if the line is empty.
*/
Queue argParser(char *line, unsigned int len);

/**
* Takes argc and argv and identifies the appropriate routine
* to create a command from. Logs the command in the command log
* and executes the command. If the command log is full, removes
* an element from the command log.
*/
int shell_execute( int argc, char *argv [] );

/**
* frees all dynamically allocated memory in a shell instance.
*/
void shell_destroy(void);

/**
* Internal Command: 
* Takes in the first argument in the format !N
* where N is an integer above 0.  This internal command looks 
* up the command in the command log and re-executes the command.
* N must be valid and within the range of the current command log.
* if the N is out of range, the shell issues a message to stderr.
*/
int bang( int argc, char * argv [] );

/**
* Internal Command:
* Print a list of internal commands with a short description.
*/
int help( int argc, char * argv [] );

/**
* Internal Command:
* Displays the command log from top to bottom, old to new.  Displays
* the S most recent commands where S is the size of the command log.
*/
int history( int argc, char * argv [] );

/**
* Internal Command:
* Frees all dynamically allocated memory on request and returns a
* value to signal shell termination to the driver.
*/
int quit( int argc, char * argv [] );

/**
* Internal Command:
* Turns verbose diagnostic [on] or [off] based on the second argument
* in argv. If the input is not in the format verbose [on|off], a usage
* message is reported to stderr.
*/
int verbose( int argc, char * argv [] );

/**
* Forks commands unknown to the shell to the unix shell. Handles errors.
*/
int external( int argc, char *argv [] );



