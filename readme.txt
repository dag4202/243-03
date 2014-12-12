//
// Name: readme.txt
// readme.txt the design details
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.3
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //

mysh.c was implemented using a combination of top to bottom to top design.  mysh.c is the driver, shell.h is the blueprint and shell.c is the implementation of the blueprint.  A 'Command' type was used to hold the commands in the command log. This 'Command' type is defined in command.h.  I decided to give the command type a parameter that pointed to a function to store the shell routine for seamless execution. A queue is used to represent the command log. The parser is a function module incorporated in the shell blueprint.  

The driver, mysh.c, does the following in order: 
It sets up the shell using a shell initializer defined in shell.h, and gets the options using a static method defined in shell.c.  Based on these options the shall may turn verbose on, or edit the size of the data structure that holds the command log (the history).  Once initialized, myshell uses getline to get individual lines and prints the sequence number from an external variable, seqNum, declared in shell.h, and defined in shell.c as an external int.  

myshell then takes the line and places it in a parser funtion declared in shell.h and defined in shell.c. This parser method splits the line into tokens and places said tokens in a queue by 3 delimiters with different priorities. Single quotes, double quotes and spaces are used as delims in that order of priority; meaning, spaces are ignored as a delimiter if within quotations. The queue is returned and mysh.c checks it to see if it is empty. If it is, then the line was empty, and the driver attempts to retrieve the next line and continue the loop.  

Otherwise, it allocates memory for a token array and for every token in the queue, the driver allocates enough memory to copy the string and place the copy in the string array.  The final string array is used to execute commands.

This token array, and its size, are passed to shell_execute to then:
1. Determine the appropriate routine to use, declared in shell.h and defined in shell.c, using the first argument of the token array.
2. Create a Command type object that stores the function pointer, the token array, its size, and the sequence number.
3. Add the command object to the command log accomodating for size.
4. Execute the routine appropriate for the command.

This makes the bang implementation much easier considering that the shell can skip a step to determine the appropriate routine to run. 

The internal and external command errors printed at their execution, and the shell manages non-fatal errors, and prevents errors that were foreseeably troublesome (but unmentioned in the assignment), such as an infinite loop/segfault that occurs when the bang routine executes another bang command and so on.

Design specifications.
