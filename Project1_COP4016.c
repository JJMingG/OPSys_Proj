#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>


void ParseIt(char* input);
void envvar(char *cmdarray);
void Path_Res(char *cmdarray);
void pipeexe(char *cmdarray);
void redirection(char *cmdarray);
void execution(char *cmdarray);
char *strrev(char *str);
int main()
{
  char *name;
  char input[26]; // array for user input
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
fgets(input, 25, stdin);
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
  if(hasPipe > 0)
	pipeexe(cmdarray);
  else if(hasRedir > 0)
	redirection(cmdarray);
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
 printf("%s", value);
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
       printf("%s", parendir);
        strcat(parendir, newpath); //newpath now contains the file pathway you need for whatever your function
        //you're using it for
        printf("%s", newpath);

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

void pipeexe(char *cmdarray){

}

void redirection(char *cmdarray){

}

void execution(char *cmdarray){

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
