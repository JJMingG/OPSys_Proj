#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct Queue {
	struct Node *front;
	struct Node *last;
	unsigned int size;
} Queue;


struct Node {
	char *cmd;
	struct Node *next;
  pid_t pid;
  int position;
};//end of node

void preppy( struct Queue *q) {
	q->front = NULL;
	q->last = NULL;
	q->size = 0;
}

void pop(struct Queue *q) {
	q->size--;

	struct Node *tmp = q->front;
	q->front = q->front->next;
	free(tmp);
//print info
}

void push(struct Queue *q, char * data) {
	q->size++;

	if (q->front == NULL) {
		q->front = (struct Node *) malloc(sizeof(struct Node));
		q->front->cmd = data;
		q->front->next = NULL;
    q->front->pid=fork();
		q->last = q->front;
    q->front->position= q->size;
	} else {
		q->last->next = (struct Node *) malloc(sizeof(struct Node));
		q->last->next->cmd = data;
    q->last->next->pid= fork();
		q->last->next->next = NULL;
		q->last = q->last->next;
    q->last->position=q->size;
	}
//print info
}

Queue background;
char * blah ="&";
preppy(background);
void execution(char **cmdline, int size);

void execution(char **cmdline, int size){
pid_t kid_pid;
pid_t tpid;
kid_pid=fork();
int kid_status;
char* pathway=getenv("PATH");
int n=0;
int b=0;
int d=sizeof(cmdline[size]);
char **tokens;
tokens=calloc(sizeof(pathway),sizeof(char*));
while(pathway[n]!=':'){
  if(pathway[n]==':') b++;

  strcat(tokens[b], pathway[n]);
  n++;
}//end of while for pathway
//run for loop for arg v to add all flages of cmdline[c][d]
if(kid_pid == 0) {
    /* child runs process. */
    for(int i=0; i< sizeof(cmdline[size]); i++) {
      char *temp;
      char **argv;
      argv= calloc(sizeof(size), sizeof(char*));
      temp=calloc(sizeof(tokens),sizeof(char*));
      for(int x=1; x <= sizeof(cmdline[size]); x++){
        if(strchr(cmdline[size][x-1],'-')!=NULL)
          argv[x]=cmdline[size][x];
      } //do argv v fill with for loop
      argv[size]=NULL;
      for(int a=0; a< sizeof(cmdline[size]); a++){
        if(tokens[a]!= blah  && a==0){
          strcat(temp, tokens[a]);
        }//ignore & at start
        else if (tokens[a]== blah && a!=0){
          if(tokens[a-1]=="|" || tokens[a-1]=="<" || tokens[a-1]==">"){
            printf("\nImproper use of background processes\n" );
            continue;
          }
          else if((a!=d-1) && (tokens[a+1]=="|" || tokens[a+1]=="<" || tokens[a+1]==">")){
            printf("\nImproper use of background processes\n" );
            continue;
          }
          else if(a> 0 && a <d-1 && tokens[a]==blah &&
         (tokens[a-1]=="|" || tokens[a-1]=="<" || tokens[a-1]==">"))
         {
           argv[0]= temp;
           if(access(temp,X_OK)==0){  //check the order of the tokens

             execv(temp,argv); //second param is args with command, flags, ends in NULL args= cmdlinflags,
             continue;
             }
           //run execution and background execution steps
          }
      }//end of else if for background processes
        argv[0]= temp;
        if(access(temp,X_OK)==0){  //check the order of the tokens
          execv(temp,argv); //second param is args with command, flags, ends in NULL args= cmdlinflags,
          continue;
          }
      }//end of nested
      free(temp);
      free(argv);
    }//end of for loop //arg0 would be the command, agr 1-infinitie  would be the flags, path from concat of $path with
    //getenv(name/path?)  use whereis to test; access/bin ls(?); look at path resolution; concat each element of the path String
    ////break path after the getenv into pieces for each /part1 /part2 , atttach the ls or whatever to it then run access.
    //if it works, the that arg0
    //getdir and getfile in each dir
    /* If execv returns, it must have failed. */
    printf("Unknown command\n");
    exit(1);
  }
  if (kid_pid == -1) {
   exit(-1);
   }

   else {
     //parent runs process
      pid_t tpid;
      tpid = wait(&kid_status);
       if(tpid != kid_pid) wait(NULL);
  }//end of else
}
//end of execution
/* start of Queue stuff for backgroun exec*/

void back_execution(char **cmdline, int size){

//waitpid with nohang and hang

/*for loop
strcmp(cmdline[i],blah)==0;// wouldn't you go for i-1 in cmdline to check for background
init (&background); */
}//end of back_execution


/*how to get file path for execv:

*/

/*how background processes work:

*/
