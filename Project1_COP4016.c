/* Progress remaining:
 * - Need parsing to parse out spaces correct and not have empty values in 2d array
 *        - some parsed lines have weird stuff at the end
          - check my parsing in redirection as a reference? might help @evan
 * - Path resolution function
 * - Execution function
 *        - Add built in function calls even tho parsit does already (mostly for pipeline an redirection)
 * - Background Processes
 * - cd function setenv
 * - io function
 * - Test each function (particulary pipe and redirection with execution and correct parsing)
 * - Create Makefile
 * - Write up README file
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>

void ParseIt(char* input);
char* envvar(char *cmdarray);
void Path_Res(char **cmdarray, int size);
void pipeexe(char **cmdline, int size, int numpipes);
void redirection(char **cmdline, int size);
void execution(char **cmdline, int size);
char *strrev(char *str);

// Built-ins
int B_exit(char **args, int size);
void cd(char **args, int size);
void echo(char **args, int size);
void etime(char **args, int size);
void io(char **args, int size);

int main()
{
  char *name;
  char input[226]; // array for user input
  char cwd[200]; // array for hold current directory
  name=(char *)malloc(10*sizeof(char));
  struct utsname uData;
  cuserid(name); // get current user
  uname(&uData); //get current machine name
//  getcwd(direc, sizeof(direc));
  printf("Bash Shell emulation initiated\n");
  printf ("%s", name);
  printf("@");
  printf("%s", uData.sysname);
  printf(" :: ");

  if (getcwd(cwd, sizeof(cwd)) == NULL){
     perror("getcwd() error");
   }
   else{
     printf("%s", cwd); // get current working directory
 }
  printf(" =>");
//while (strcmp(input, "exit") != 0){ //checks for exit cmd
while(1){
//  getcwd(direc, sizeof(direc)); // ignore for now
  cuserid(name);
fgets(input, 225, stdin);
ParseIt(input); // calls parsing function seen below
printf ("%s", name);
printf("@");
uname(&uData);
printf("%s", uData.sysname);
printf(" :: ");
if (getcwd(cwd, sizeof(cwd)) == NULL){
   perror("getcwd() error");
 }
 else{
   printf( "%s", cwd);
//fprintf(stdout, "%s", direc);
        }
printf(" =>");
    }
	return 0;
}

void ParseIt(char* input){
  int hasPipe = 0, hasRedir = 0; // check signals for if there is pipelining or redirection
  char cmdarray[256] = {' '};
  int cmd_array_counter = 0;
  int amp_counter = 0;
for (int i = 0; i < strlen(input); i++){// loop through the entire input in order to parse
  if (input[0] == '&' && amp_counter == 0){
    ++amp_counter;
    continue;
  }
if(input[i] == '|' ||input[i] == '<' || input[i] == '>' || input[i] == '&'){ // look for special characters

/* Checks for pipelines and redirections */
  if(input[i] == '|')
	hasPipe++;
  else if(input[i] == '<' || input[i] == '>') //using else if because we can assume pipes and I/O redirection wont occur together
	hasRedir++;

  if(input[i - 1] == ' '){
//cmdarray[cmd_array_counter - 1] = '*';
cmdarray[cmd_array_counter] = input[i];
cmd_array_counter++;
//printf("this ran");
  }
  else{
cmdarray[cmd_array_counter] =  '*';
cmdarray[++cmd_array_counter] = input[i]; // In case the user adds no spaces between commands
if(&input[i + 1] == NULL){ // if end of input break
break;
}
else{
cmdarray[++cmd_array_counter] = '*';
}
cmd_array_counter++;
  }
}
else{
  cmdarray[cmd_array_counter] = input[i];
  if (input[i] == ' '){
cmdarray[cmd_array_counter] = '*'; // add an asterisk for every space
  }
  cmd_array_counter++;
}
    }
    printf("%s", cmdarray);
    char ** cmdline;
    char * temp;
    int checker = 0;
    int looker = 0;
    int looker_two = 0;
    int size = 0;
    int test = 0;
    int make = 0;
    cmdline = (char **)calloc(strlen(cmdarray), sizeof(char *));
while(test == 0){
  temp = (char *)calloc(strlen(cmdarray), sizeof(char));
for(int i = looker; i < strlen(cmdarray); i++){
  if(cmdarray[i] != '*'){
make = 1;
  }
  if (make == 1){
  if(i == strlen(cmdarray) || i  == (strlen(cmdarray) - 1)){
  //  printf("This ran");
    test = 1;
  }
  if(cmdarray[i] == '*'){
    checker = 0;
    ++looker;
    break;
    }
  else
    {
    temp[checker] = cmdarray[i];
    checker++;
    }
  looker++;
  }
}
  //printf("%s\n", temp);
cmdline[size] = temp;
printf("%s\n", cmdline[size]);
size++;


}

   //performs any of the environment variable needs if there are any
//  printf("%s", cmdarray); // print statement for confirmation of correct parsing
  Path_Res(cmdline, size);

  /* Execution process commands */
  if(hasPipe > 0){
	   pipeexe(cmdline, size, hasPipe);
	   hasPipe = 0;
  }
  else if(hasRedir > 0){
	   redirection(cmdline, size);
	   hasRedir = 0;
  }
  else if(strcmp(cmdline[0], "exit") == 0) // not sure why this only works with exit with a space
     B_exit(cmdline, size);
  else if(strcmp(cmdline[0], "echo") == 0)
     echo(cmdline, size);
  else if(strcmp(cmdline[0], "etime") == 0)
     etime(cmdline, size);
  else if(strcmp(cmdline[0], "io") == 0)
     io(cmdline, size);
  else if(strcmp(cmdline[0], "cd") == 0)
     cd(cmdline, size);
  else
	   execution(cmdline, size);
  /* Will need built-ins in execution function too */
}

char* envvar(char *cmdarray){
char env_var[200];
int a = 0;
for (int i = 0; i < strlen(cmdarray); i++){
  if(cmdarray[i] == '$'){
    for (int b = i; b < strlen(cmdarray); b++){
      if(cmdarray[b] == '*' || cmdarray[b] == ' '){ //PARSE out the env_var so you can look it up using getenv
        break;
            }
      env_var[a] = cmdarray[b + 1];
      a++;
      }
    }
  }
   printf("%s", env_var);
char value[150] = {' '};
char *env_value;
for(int i = 0;i < strlen(env_var) - 1; i++) {
value[i] = env_var[i]; //have to get rid of null character because its a c string
 }
 printf("%s\n", value);
 env_value = getenv(value); //Env value is saved in env_value if needed when you use it or you need to echo it
  //printf(env_value);
  return env_value;
}

void Path_Res(char **cmdline, int size){
  //printf("%s", cmdarray);
  for (int i = 0; i < (size); i++){
    for(int a = 0; a < strlen(&cmdline[i][a]); a++){
    if(cmdline[i][a] == '.'){
    //DO nothing since we're already in this current directory
    }
    if(cmdline[i][a] == '.' && cmdline[i][a+1] == '.'){
      char cwd[200];
      if (getcwd(cwd, sizeof(cwd)) == NULL){
         perror("getcwd() error");
        }
       else{
        //printf( "%s", cwd);
       //fprintf(stdout, "%s", direc);
     }
     for(int b = 0; b < strlen(cmdline[i]); b++){



     }
  }
    if(cmdline[i][a] == '~'){

//replace beginning with $HOME environmental variable

    }
    if(cmdline[i][a] == '/'){


    }
    if(cmdline[i][a] == '&' || cmdline[i][a] == '<' || cmdline[i][a] == '*' || cmdline[i][a] == '>'){
        i++;
    }
    else{
//Make this look and see if it a is a lone file, if not check it its one of the four commands io, echo, exit or etime
// If none of those signal a file not found error

      }
    }
  }
}

void pipeexe(char **cmdline, int size, int numpipes){
	/* parse based on pipelines */
	int index[numpipes + 2]; // +2 to compensate the begining and end which shouldn't have pipes.
	int indexcounter = 1;

	index[0] = -1;
	index[numpipes + 1] = size;

	// Iterate through cmdline to find index of pipes
	for(int i = 0; i < size; i++){
		if(strcmp(cmdline[i], "|") == 0){
			index[indexcounter] = i;
			indexcounter++;
		}
	}

  /* Error checking */
	// Not sure if i shud do error checking here or in the actual implementation but I'll leave it here for now
	for(int i = 0; i < numpipes + 1; i++){
		if(index[i + 1] - index[i] <= 1){
			perror("Error: syntax error with pipes. Exiting...\n");
			exit(1);
		}
	}

	/* Passing executions before and after pipes would be
	 * for(int i = 0, i < numpipes + 1; i++)
	 * execution(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
	 *
	 * The cmdline + (1 + index[i]) shud pass the cmdline 2d array starting from the beginning of command of each pipe
	 * The +1 is to skip over the "|"
	 * The index[i + 1] - index[i] - 1 is to indicate the size which in reality is bigger but this will show only the parts
	 * that are before the next pipe
	 */
	/* end of parsing pipelines */

	/* Implementation */
  int fd1[2];
  int fd2[2];
	int pid[numpipes + 1];

	for(int i = 0; i < numpipes + 1; i++){
    if(i == 0){ // First Command
          pipe(fd1);
      		if(pid[i] = fork() == 0){ // Child (cmd1 | cmd2)
              close(STDOUT_FILENO);
              dup(fd1[1]);
              close(fd1[0]);
              close(fd1[1]);

              // Execution
              //execution(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
              echo(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
              exit(0);
           }
           else if(pid[i] < 0){
              perror("Pipe error");
              exit(0);
      		 }
     }
     else if(i == numpipes){ // Last Command
           pipe(fd2);
           if(pid[i] = fork() == 0){ // Child (cmd1 | cmd2)
               close(STDIN_FILENO);
               dup(fd2[0]);
               close(fd2[0]);
               close(fd2[1]);

               // Execution
               //execution(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
               echo(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
               exit(0);
            }
            else if(pid[i] < 0){
               perror("Pipe error");
               exit(0);
            }
      }
      else{
            pipe(fd1);
            pipe(fd2);
            if(pid[i] = fork() == 0){ // Child (cmd1 | cmd2)
                // Get output as input
                close(STDOUT_FILENO);
                dup(fd1[1]);
                close(fd1[0]); // might need to close after the next dup
                close(fd1[1]);
                // Get info as output for next cmd
                close(STDIN_FILENO);
                dup(fd2[0]);
                close(fd2[0]);
                close(fd2[1]);

                // Execution
                //execution(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
                echo(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
                exit(0);
             }
             else if(pid[i] < 0){
                perror("Pipe error");
                exit(0);
             }
      }
	}
  for(int i = 0; i < numpipes + 1; i++){ // Parent
       waitpid(pid[i], NULL, 0);
  }
  /* end of pipe implementation */
}

void redirection(char **cmdline, int size){
    printf("IN REDIRECTION FUNCTION\n");
    /* Indexing of redirections */
    int index = 0; // Only need index for 1 redirection as stated by WenQi
    for(int i = 0; i < size; i++){
       if(strcmp(cmdline[i], "<") == 0 || strcmp(cmdline[i], ">") == 0){
          if(index != 0){
              // Keep even after finding index, in case there's another redirection symbol
              perror("Error: Cannot have more than one redirection symbol");
              exit(0);
          }
          index = i;
       }
    }
    printf("AFTER INDEXING\n");
    /* Error Handling */
    if(index == 0){
        perror("Error: No command specified");
        exit(0);
    }
    else if(index == size - 1){
        perror("Error: No file specified");
        exit(0);
    }
    printf("AFTER ERROR CHECKING\n");
    /* Parse out redirection symbol and filename */
    char ** temp = (char **)calloc(size - 2, sizeof(char *));
    for(int i = 0; i < index; i++){ // get the cmds and args before the redirection symbol
        temp[i] = (char *)calloc(strlen(cmdline[i]) + 1, sizeof(char));
        temp[i] = cmdline[i];
    }
    for(int i = index + 2; i < size; i++){ // get the args after filename b/c they are considered args too
        temp[i] = (char *)calloc(strlen(cmdline[i]) + 1, sizeof(char));
        temp[i] = cmdline[i];
    }
    printf("AFTER PARSING\n");
    /* Implementation */
    int fd, pid;

    if(pid = fork() == 0){ // child process
        if(strcmp(cmdline[index], ">") == 0){ // Output Redirection
            // Create file if it doesn't exist, overwrite if it does
            fd = open(cmdline[index + 1], O_CREAT | O_TRUNC | O_WRONLY);
            close(STDOUT_FILENO);
            dup(fd);
            close(fd);
        }
        else if(strcmp(cmdline[index], "<") == 0){ // Input Redirection
            fd = open(cmdline[index + 1], O_RDONLY);
            close(STDIN_FILENO);
            dup(fd);
            close(fd);
        }
        else{
            // Shouldn't be reached but here for safety measures
            perror("Redirection implementation error");
            exit(0);
        }

        // Execution
        //execution(2dtemp, size - 2);
        echo(temp, size - 2);
    }
    else if(pid < 0){
        perror("Fork error in redirection function");
        exit(0);
    }
    else{ // parent process
        close(fd);
    }
    printf("AFTER IMPLEMENTATION\n");
    /* Memory Clean-up */
    for(int i = 0; i < size - 2; i++){
        printf("I is %d", i);
        free(temp[i]);
    }
    free(temp);
}

void execution(char **cmdline, int size){


}

/* Built-ins */
int B_exit(char **args, int size){
	printf("Exiting Shell....\n");
	exit(0);
}

void cd(char **args, int size){
  printf("size is %d\n", size);
  args[1] = envvar(args[1]);
  for(int i = 0; i < size; i++)
    printf("content: %s\n", args[i]);
	if(size < 2){ // If no args, $HOME is the arg
		//char * home = "$HOME"; // String for $HOME
	   args[1] = envvar("$HOME"); // Pass through env_var and copy to args[1]
	}
	 // Signals error if target is not a directory
  // Path resolution is predetermined already
	if(chdir(args[1]) != 0){
	   perror("Error");
     B_exit(args, size);
	}
  //if(setenv(envvar("$PWD"), args[1], 1) != 0){
     //perror("Unable to set PWD");
  //}
}

void echo(char **args, int size){
	//outputs whatever user specifies
	//for each argument:
		//look up the argument in the list of environment variables
		//print the value if it exists
		//signal an error if it does not exist
    for(int i = 1; i < size; i++){
      if (strcmp(args[i], "$") == 0){ // environmental variable
        printf("%s ", envvar(args[i + 1])); // envvar will print out error message if it does not exist
        i++; // arg is in next index after the $ so increemnt to skip
      }
      else
        printf("%s ", args[i]);
    }
    printf("\n");
}

void etime(char **args, int size){
  long sec, msec;
	struct timeval start, end;
	gettimeofday(&start, NULL);

	// take out "etime" and pass to execute function
  execution(args + 1, size - 1);
  // passes in by starting from the next command and decrease size to compensate

	gettimeofday(&end, NULL);
  sec = end.tv_sec - start.tv_sec;
  msec = end.tv_usec - start.tv_sec;
  if(msec < 0){
      sec -= 1;
      msec += 1;
  }
	printf("Elapsed Time: %ld.%06ld\n", sec, msec); //end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
}

void io(char **args, int size){

}






























































char *strrev(char *str)
{
    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}







/*
char newpath[250] = {};
char cwd[200] = {};
char parendir[200] = {};
int trackerforparen = 0;
int backslashtrack = 0;
for (int b = 0; b < strlen(cmdarray); b++){
newpath[b] = cmdarray[i + 2];
if(cmdarray[i] == '*' || cmdarray[i] == ' ' || &cmdarray[i] == NULL){
  break;
      }
      i++;//remove first to top dots
    }
    //printf("%s", newpath);
   if (getcwd(cwd, sizeof(cwd)) == NULL){
      perror("getcwd() error");
     }
    else{
     //printf( "%s", cwd);
    //fprintf(stdout, "%s", direc);
  }
  for(int i = strlen(cwd) - 1; i > 0; i--){
      parendir[trackerforparen] = cwd[i]; //get currentdir and parent directory
      trackerforparen++;
      if(cwd[i] == '/'){
        ++backslashtrack;
      }
      if (backslashtrack == 2){
        break;
      }
  }
  //printf("this ran");
  //printf("%s\n", cwd);
  strrev(parendir);
 printf("%s", parendir);
  strcat(parendir, newpath); //newpath now contains the file pathway you need for whatever your function
  //you're using it for
  printf("%s", newpath);*/
