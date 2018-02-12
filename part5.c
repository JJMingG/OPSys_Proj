void execution(char **cmdline, int size){
    printf("IN EXECUTION FUNCTION - Tested and functional, maybe test when path resolution works\n");
    int pid;
    int count=0;
    struct Node* head = NULL; // used for background process
    if(strcmp(cmdline[size], "&") == 0){
        cmdline[size][sizeof(cmdline[size])]="";
        push(&head,cmdline[0], pid, count);
        count++;
      }
      //modify the while loop to pritn out "start execution"
      while((pid = waitpid(-1, &count, WNOHANG)) > 0) {
          printf("[%d]+  [%d]\n",
                  WEXITSTATUS(count),pid);
               }
    if(pid = fork() == 0){ // Child process
        /* - List of Execution Processes -
         * Built-ins are checked again despite being checked for in the parsing
         * function. This is mainly for other functions such as pipeexe,
         * redirection, and built-ins.
         */

        if(strcmp(cmdline[0], "exit") == 0)
            B_exit(cmdline, size);
        else if(strcmp(cmdline[0], "echo") == 0)
            echo(cmdline, size);
        else if(strcmp(cmdline[0], "etime") == 0)
            etime(cmdline, size);
        else if(strcmp(cmdline[0], "io") == 0)
            io(cmdline, size);
        else if(strcmp(cmdline[0], "cd") == 0)
            cd(cmdline, size);

        else{ // Execute external commands
            execv(cmdline[0], cmdline);
            // Reaches here only if execution fails
            printf("Problem executing %s\n", cmdline[0]);
            exit(0);
        }
    }
    else if(pid < 0){ // Error forking
        perror("Forking error in execution function\n");
        exit(0);
    }
    else{ // Parent process
        waitpid(pid, NULL, 0);
    }
    while((pid = waitpid(-1, &count, WNOHANG)) > 0) {
        /*printf("[%d]+  [%d]\n",
                WEXITSTATUS(count),pid);
          */
        //call background process function instead and use it to pop LL }
    // printf()
}

void background(char **cmdline, int size){
  while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
      printf("[proc %d exited with code %d]\n",
             pid, WEXITSTATUS(status));
      /* here you can remove the pid from your jobs list */
  }

}
