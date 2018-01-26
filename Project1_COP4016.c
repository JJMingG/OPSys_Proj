#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
main()
{
  char *name;
  char input[15];
  char cwd[200];
  name=(char *)malloc(10*sizeof(char));
  struct utsname uData;
  cuserid(name);
  uname(&uData);
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
     printf("%s", cwd);
 }
  printf(" =>");
while (strcmp(input, "exit") != 0){
//  getcwd(direc, sizeof(direc));
  cuserid(name);
fgets(input, 15, stdin);
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
