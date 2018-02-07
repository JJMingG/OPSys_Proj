void execution(char **cmdline, int size){
pid_t kid_pid;
pid_t tpid;
kid_pid=fork();
int kid_status;
if(kid_pid == 0) {
    /* child runs process. */
    execv(argv[0], argv); //arg0 would be the command, agr 1-infinitie  would be the flags, path from concat of $path with
    //getenv(name/path?)  use whereis to test; access/bin ls(?); look at path resolution; concat each element of the path String
    ////break path after the getenv into pieces for each /part1 /part2 , atttach the ls or whatever to it then run access.
    //if it works, the that arg0
    /* If execv returns, it must have failed. */
    printf("Unknown command\n");
    exit(1);
  }
  if (kid_pid == -1) {
   exit(1);
   }

   else {
     //parent runs process
     do {
       pid_t tpid = wait(&kid_status);
       if(tpid != kid_pid) process_terminated(tpid);
     } while(tpid != kid_pid);

     return kid_status;
  }//end of else
}
//end of execution
/* start of Queue stuff for backgroun exec*/
struct Node {
	char cmd[200];
	struct Node *next;
  pid_t pid;
  int position;
}

struct Queue {
	struct Node *front;
	struct Node *last;
	unsigned int size;
};

void init(struct Queue *q) {
	q->front = NULL;
	q->last = NULL;
	q->size = 0;
}

void pop(struct Queue *q) {
	q->size--;

	struct Node *tmp = q->front;
	q->front = q->front->next;
	free(tmp);
}

void push(struct Queue *q, char * data) {
	q->size++;

	if (q->front == NULL) {
		q->front = (struct Node *) malloc(sizeof(struct Node));
		q->front->cmd = data;
		q->front->next = NULL;
    q->front->pid=fork();
		q->last = q->front;
    q->position= size;
	} else {
		q->last->next = (struct Node *) malloc(sizeof(struct Node));
		q->last->next->cmd = data;
    q->last->next->pid= fork();
		q->last->next->next = NULL;
		q->last = q->last->next;
    q->position=size;
	}
}

void back_execution(char **cmdline, int size){
struct Queue background;
char * blah ="&";
//waitpid with nohang and hang
/*for loop
strcmp(cmdline[i],blah)==0;// wouldn't you go for i-1 in cmdline to check for background
init (&background); */
}//end of back_execution


/*how to get file path for execv:

*/

/*how background processes work:

*/
