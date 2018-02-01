#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>

void ParseIt(char* input);
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
//printf("%s", input);
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
  char cmdarray[256] = {' '};
  int cmd_array_counter = 0;
for (int i = 0; i < strlen(input); i++){
  if (input[0] == '&'){
    i++;
  }
if(input[i] == '|' ||input[i] == '<' || input[i] == '>' || input[i] == '&'){
  if(input[i - 1] == ' '){
//cmdarray[cmd_array_counter - 1] = '*';
cmdarray[cmd_array_counter] = input[i];
cmd_array_counter++;
//printf("this ran");
  }
  else{
cmdarray[cmd_array_counter] =  '*';
cmdarray[++cmd_array_counter] = input[i];
if(&input[i + 1] == NULL){
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
cmdarray[cmd_array_counter] = '*';
  }
  cmd_array_counter++;
}
    }
    printf("%s", cmdarray);
}
