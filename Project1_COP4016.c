#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
main()
{
  char *name;
  char input[15];
  name=(char *)malloc(10*sizeof(char));
  struct utsname uData;
  cuserid(name);
  uname(&uData);
  printf("Bash Shell emulation initiated\n");
  printf ("%s", name);
  printf("@");
  printf("%s", uData.sysname);
  printf(" :: =>");
while (strcmp(input, "exit") != 0){
  cuserid(name);
fgets(input, 15, stdin);
printf ("%s", name);
printf("@");
uname(&uData);
printf("%s", uData.sysname);
printf(" :: =>");

}
}
