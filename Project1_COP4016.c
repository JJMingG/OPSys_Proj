#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/time.h>

void ParseIt(char* input);
char* envvar(char *cmdarray);
void Path_Res(char **cmdarray, int size);
void pipeexe(char **cmdline, int size, int numpipes);
void redirection(char **cmdline, int size);
void execution(char *cmdarray);
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
while (strcmp(input, "exit") != 0){ //checks for exit cmd
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
    cmdline = (char **)calloc(strlen(cmdarray), sizeof(char *));
while(test == 0){
  temp = (char *)calloc(strlen(cmdarray), sizeof(char));
for(int i = looker; i < strlen(cmdarray); i++){
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
  else
	execution(cmdarray);
  /* Because pipelining and redirection both needs execution, it is better off sending them to their own function
   * to do their respective parts and then call execution. Built-in functions and background processing will also
   * need execution.
   */
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
	printf("In pipe function\n");
	/* parse based on pipelines */
	char *** cmds;
	int index[numpipes];
	int indexcounter = 0;
	char * pipelinechar = "|";

	for(int i = 0; i < size; i++)
	{
		printf("Checking args: %s\n", cmdline[i]);
		if(strcmp(cmdline[i], pipelinechar) == 0){
			index[indexcounter] = i;
			indexcounter++;
			printf("Pipeline at %d\n", i);
		}
	}

/*
	char ** cmds;
	cmds = (char **)malloc(sizeof(char *) * (size + 1));
	int * index = (int*)malloc(sizeof(int) * (size + 2));
	int * spaces = (int*)malloc(sizeof(int) * (size + 1));
	index[0] = -1;
	index[size + 1] = strlen(cmdarray) - 1;
	int indexcount = 1, spacecount = 0;

	for(int i = 0; i < strlen(cmdarray); i++){ // finds index of pipelines to parse
		if(cmdarray[i] == '*') // counts number of spaces between each pipe
			spacecount++;
		if(cmdarray[i] == '|'){
			index[indexcount] = i;
			indexcount++;
			spaces[indexcount - 2] = spacecount;
			spacecount = 0;
		}
	}
	spaces[size] = spacecount;

	for(int i = 0; i < size + 1; i++){
		cmds[i] = (char *)malloc(sizeof(char) * (index[i + 1] - index[i] + 1));
		strncpy(cmds[i], &cmdarray[index[i] + 1], (index[i + 1] - index[i] - 1));
		cmds[i][index[i + 1] - index[i] - 1] = '\0'; // null terminating
	}
	index[0] = 0;
*/
	/* end of parsing pipelines */

	/* Error checking 
	for(int i = 0; i < size + 1; i++){
		if(index[i + 1] - index[i] - spaces[i] <= 1){ // Doesn't work for 1 letter commands but there shudnt be such a thing anyways
			printf("Error: syntax error with pipes. Exiting...\n");
			exit(1);
		}
	}
*/
	/* Implementation */
/* Skeleton of the implementation straight from the book. Commented out for now
	int fd[2];
	int pid, pid2;

	for(int i = 1; i < size + 1; i++){
		if(pid = fork() == 0){
			// Child (cmd1 | cmd2)
			pipe(fd);
			if(pid2 = fork() == 0){ // fork == 0 is in child process
				// cmd1 (Writer)
				close(STDOUT_FILENO);
				dup(fd[1]);
				close(fd[0]);
				//close(fd[1]);
				// Execute Command (use command function)
				printf("Child Execution: cmds[i] using cmds[i - 1] (%s): %s\n", cmds[i - 1], cmds[i]);
			}
			else if(pid2 < 0){ // for < 0 is error
				perror("fork2");
				exit(1);
			}
			else{ // fork > 0 is in parent process
				// cmd2 (Reader)
				close(STDIN_FILENO);
				dup(fd[0]);
				//close(fd[0]);
				close(fd[1]);
				// Execute Command (use command function)
				printf("Parent Execution: cmds[i - 1]: %s\n", cmds[i]);
			}
		}
		else if(pid < 0){
			perror("fork1");
			exit(1);
		}
		else{
			// Parent (Shell)
			//close(fd[0]);
			//close(fd[1]);
			//close(fd);
			printf("IN PARENT SHELL and i is %d\n", i);
			//waitpid();
		}
	}
*/
	/* TEST AREA */
/*	for(int i = 0; i < size + 1; i++)
		printf("%s\nlength: %d\n", cmds[i], strlen(cmds[i]));
*/
	/* Clean-up 
	for(int i = 0; i < size + 1; i++)
		free(cmds[i]); // not sure if free(cmds) would free these too
	free(index);
	free(spaces);
	free(cmds);
*/
}

void redirection(char **cmdline, int size){

}

void execution(char *cmdarray){


}

/* Built-ins */
int B_exit(char **args, int size){
	printf("Exiting Shell....");
	return 0;
}

void cd(char **args, int size){
	// This function is assuming an array of char arrays is passed in
	if(args[1] == NULL){ // If no args, $HOME is the arg
		//char * home = "$HOME"; // String for $HOME
		//strcpy(args[1], envvar(home)); // Pass through env_var and copy to args[1]
	}
	else{ // Signals error if target is not a directory
		//strcpy(args[1], Path_Res(args[1])); // pass arg to get path resolution
		if(chdir(args[1]) != 0)
			perror("Not a directory");
	}
}

void echo(char **args, int size){
	//outputs whatever user specifies
	//for each argument:
		//look up the argument in the list of environment variables
		//print the value if it exists
		//signal an error if it does not exist
}

void etime(char **args, int size){
	struct timeval start, end;
	gettimeofday(&start, NULL);

	// take out "etime" and pass to execute function

	gettimeofday(&end, NULL);
	printf("Elapsed Time: %ld.%06ld\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
	// Not sure if subtracting the microseconds will return a negative at times?
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
