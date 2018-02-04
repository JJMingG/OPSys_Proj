#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>


void ParseIt(char* input);
void envvar(char *cmdarray);
void Path_Res(char *cmdarray);
void pipeexe(char *cmdarray, int size);
void redirection(char *cmdarray);
void execution(char *cmdarray);
char *strrev(char *str);
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

   //performs any of the environment variable needs if there are any
  printf("%s", cmdarray); // print statement for confirmation of correct parsing
  envvar(cmdarray);
  Path_Res(cmdarray);

  /* Execution process commands */
  if(hasPipe > 0){
	pipeexe(cmdarray, hasPipe);
	hasPipe = 0;
  }
  else if(hasRedir > 0){
	redirection(cmdarray);
	hasRedir = 0;
  }
  else
	execution(cmdarray);
  /* Because pipelining and redirection both needs execution, it is better off sending them to their own function
   * to do their respective parts and then call execution. Built-in functions and background processing will also
   * need execution, and will need to be created within the execution function.
   */
}

void envvar(char *cmdarray){
char env_var[20] = {' '};
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
}

void Path_Res(char *cmdarray){
  //printf("%s", cmdarray);
  char newpath[250] = {};
  char cwd[200] = {};
  char parendir[200] = {};
  int trackerforparen = 0;
  int backslashtrack = 0;
  for (int i = 0; i < strlen(cmdarray); i++){
    if(cmdarray[i] == '.'){
    //DO nothing since we're already in this current directory
    }
    if(cmdarray[i] == '.' && cmdarray[i+1] == '.'){
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
      //  printf("%s", parendir);
        strcat(parendir, newpath); //newpath now contains the file pathway you need for whatever your function
        //you're using it for
        //printf("%s", newpath);

      }
    if(cmdarray[i] == '~'){

//replace beginning with $HOME environmental variable

    }
    if(cmdarray[i] == '/'){


    }
    if(cmdarray[i] == '&' || cmdarray[i] == '<' || cmdarray[i] == '*' || cmdarray[i] == '>'){
        i++;
    }
    else{
//Make this look and see if it a is a lone file, if not check it its one of the four commands io, echo, exit or etime
// If none of those signal a file not found error

    }
  }
}

void pipeexe(char *cmdarray, int size){
	printf("In pipe function\n");
	/* parse based on pipelines */
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
	/* end of parsing pipelines */

	/* Error checking */
	for(int i = 0; i < size + 1; i++){
		if(index[i + 1] - index[i] - spaces[i] <= 1){ // Doesn't work for 1 letter commands but there shudnt be such a thing anyways
			printf("Error: syntax error with pipes. Exiting...\n");
			exit(1);
		}
	}

	/* Implementation */
/* Skeleton of the implementation straight from the book. Commented out for now 
	int fd[2];

	for(int i = 1; i < size + 1; i++){
		if(fork() == 0){
			// Child (cmd1 | cmd2)
			pipe(fd);
			if(fork() == 0){ // fork == 0 is in child process
				// cmd1 (Writer)
				close(STDOUT_FILENO);
				dup(fd[1]);
				close(fd[0]);
				//close(fd[1]);
				// Execute Command (use command function)
				printf("Child Execution: cmds[i] using cmds[i - 1] (%s): %s\n", cmds[i - 1], cmds[i]);
			}
			else{ // fork > 0 is in parent process, fork < 0 is error
				// cmd2 (Reader)
				close(STDIN_FILENO);
				dup(fd[0]);
				//close(fd[0]);
				close(fd[1]);
				// Execute Command (use command function)
				printf("Parent Execution: cmds[i - 1]: %s\n", cmds[i - 1]);
			}
		}
		else{
			// Parent (Shell)
			//close(fd[0]);
			//close(fd[1]);
			close(fd);
			printf("IN PARENT SHELL and i is %d\n", i);
			//waitpid();
		}
	}
*/
	/* TEST AREA */
/*	for(int i = 0; i < size + 1; i++)
		printf("%s\nlength: %d\n", cmds[i], strlen(cmds[i]));
*/
	/* Clean-up */
	for(int i = 0; i < size + 1; i++)
		free(cmds[i]); // not sure if free(cmds) would free these too
	free(index);
	free(spaces);
	free(cmds);
}

void redirection(char *cmdarray){

}

void execution(char *cmdarray){
// Background Processing and Built-ins will need to be implemented here i believe

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
