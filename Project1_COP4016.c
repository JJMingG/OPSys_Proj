/* Progress remaining:
 * - Need parsing to parse out spaces correct and not have empty values in 2d array
 *        - cmd (cmd + space) creates two pointers instead of only one
 * - Path resolution function
 * - Background Processes
 * - Test each function, particularly:
 *        - cd (setenv and when path resolution is done)
 *        - envvar FUNCTION
 *        - Path resolutions
 *        - background processes
 *        - execution with path resolutions
 *        - spacing with functions
 * - Create Makefile
 * - Write up README file
 * - Clean up comments, printf, etc
 * - Get rid of warnings if possible, currently 2
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
void envvar(char **cmdline, int size);
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

int main(){
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
    int i = 0;
    int counter = 0;
    int numofcommand = 0;
    int size = 0;
    int cmd1counter = 0;
    int cmd2counter = 0;
    int cmd3counter = 0;
    int cmd4counter = 0;
    int cmd5counter = 0;
    int cmd6counter = 0;
    int cmd7counter = 0;
    int cmd8counter = 0;
    int cmd9counter = 0;
    int cmd10counter = 0;
    int cmd11counter = 0;
    char * cmd1 = (char *)calloc(15, sizeof(char *));
    char * cmd2 = (char *)calloc(15, sizeof(char *));
    char * cmd3 = (char *)calloc(15, sizeof(char *));
    char * cmd4 = (char *)calloc(15, sizeof(char *));
    char * cmd5 = (char *)calloc(15, sizeof(char *));
    char * cmd6 = (char *)calloc(15, sizeof(char *));
    char * cmd7 = (char *)calloc(15, sizeof(char *));
    char * cmd8 = (char *)calloc(15, sizeof(char *));
    char * cmd9 = (char *)calloc(15, sizeof(char *));
    char * cmd10 = (char *)calloc(15, sizeof(char *));
    char * cmd11 = (char *)calloc(15, sizeof(char *));
    cmdline = (char **)calloc(strlen(cmdarray), sizeof(char **));

    while(i != strlen(cmdarray)){
        if(cmdarray[i] == '*'){
        }
        else if(numofcommand == 0){
            cmd1[cmd1counter] = cmdarray[i];
            cmd1counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd1;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 1){
            cmd2[cmd2counter] = cmdarray[i];
            cmd2counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd2;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 2){
            cmd3[cmd3counter] = cmdarray[i];
            cmd3counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){ //Seperate Pointer for each spot in the array
                cmdline[size] = cmd3;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 3){
            cmd4[cmd4counter] = cmdarray[i];
            cmd4counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd4;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 4){
            cmd5[cmd5counter] = cmdarray[i];
            cmd5counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd5;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 5){
            cmd6[cmd6counter] = cmdarray[i];
            cmd6counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd6;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 6){
            cmd7[cmd7counter] = cmdarray[i];
            cmd7counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd7;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 7){
            cmd8[cmd8counter] = cmdarray[i];
            cmd8counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd8;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 8){
            cmd9[cmd9counter] = cmdarray[i];
            cmd9counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd9;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 9){
            cmd10[cmd10counter] = cmdarray[i];
            cmd10counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd10;
                size = size + 1;
                numofcommand++;
            }
        }
        else if(numofcommand == 10){
            cmd11[cmd11counter] = cmdarray[i];
            cmd11counter++;
            if(cmdarray[i + 1] == '*' || i == (strlen(cmdarray) - 1)){
                cmdline[size] = cmd11;
                printf("%s", cmdline[i]);   size = size + 1;
                numofcommand++;
            }
        }
        i++;
    }

    char * temp = (char *)calloc(15, sizeof(char *));
    int another_counter = 0;
    for(int i = 0; i < size ;i++){
        if(cmdline[i+1] == NULL || strcmp(cmdline[i+1], " ") == 0){
            printf("%s", cmdline[i]);
            strncpy(temp, cmdline[i], (strlen(cmdline[i]) - 1));
            cmdline[i] = temp;
        }
    }

    // Call path resolution and env_var function to predefine arguments
    Path_Res(cmdline, size);
    envvar(cmdline, size);
    printf("Testing cmdline[z]\n");
    for(int z = 0; z < size; z++)
        printf("cmdline[z]: %s\n", cmdline[z]);

    /* Execution process commands */
    if(strcmp(cmdline[0], "exit") == 0)
        B_exit(cmdline, size);
    else if(hasPipe > 0){
  	    pipeexe(cmdline, size, hasPipe);
  	    hasPipe = 0;
    }
    else if(hasRedir > 0){
  	    redirection(cmdline, size);
  	    hasRedir = 0;
    }
    else if(strcmp(cmdline[0], "/usr/bin/echo") == 0 || strcmp(cmdline[0], "echo") == 0)
        echo(cmdline, size);
    else if(strcmp(cmdline[0], "etime") == 0)
        etime(cmdline, size);
    else if(strcmp(cmdline[0], "io") == 0)
        io(cmdline, size);
    else if(strcmp(cmdline[0], "/user/bin/cd") == 0 || strcmp(cmdline[0], "cd") == 0)
        cd(cmdline, size);
    else
  	    execution(cmdline, size);
}

void envvar(char **cmdline, int size){
    printf("IN ENV_VAR - need to completely test\n");
    for(int i = 0;i < size;i++){
        if(cmdline[i][0] == '$'){
            char * temp = (char *)calloc(strlen(cmdline[i]), sizeof(char));
            strncpy(temp, cmdline[i] + 1, strlen(cmdline[i]));
            printf("%s\n", temp);
            temp = getenv(temp);
            printf("HERE\n");
            cmdline[i]  = temp;
            printf("HERE2\n");
            //free(temp);
            printf("HERE3\n");
            printf("%s\n", cmdline[i]);
            printf("HERE4\n");
        }
    }
}

void Path_Res(char **cmdline, int size){
    printf("In path resolution - Not finished and need to test\n");
    int done = 0;
    int cmdline_counter_double_dot = 0;
    int cmdline_counter_single_dot = 0;
    int cmdline_tilda = 0;
    char *Pwd_holder_double_dot = (char *)calloc(15, sizeof(char *));
    char *Pwd_holder_single_dot = (char *)calloc(15, sizeof(char *));
    char *Pwd_holder_tilda = (char *)calloc(15, sizeof(char *));
    char *checkit = (char *)calloc(15, sizeof(char *));
    char *checkit_tilda = (char *)calloc(15, sizeof(char *));
    for(int i = 0; i < size;i++){
        for(int a = 0; a < strlen(cmdline[i]); a++){
            if(cmdline[i][a] == '.' && cmdline[i][a+1] == '.'){
                for(int b = 0; b < (strlen(cmdline[i]) + 1); b++){
                    if(cmdline[i][b] == '.'){
                    }
                    else {
                        cmdline[i][cmdline_counter_double_dot] = cmdline[i][b];
                        cmdline_counter_double_dot++;
                    }
                }
                Pwd_holder_double_dot = getenv("PWD");
                strrev(Pwd_holder_double_dot);
                int back_slash = 0;
                int checkit_counter = 0;
                for(int b = 0;b < strlen(Pwd_holder_double_dot); b++){
                    if(Pwd_holder_double_dot[b] == '/'){
                        back_slash++;
                    }
                    if(back_slash > 0){
                        checkit[checkit_counter] = Pwd_holder_double_dot[b + 1];
                        checkit_counter++;
                    }
                }
                strrev(checkit);
                strcat(checkit, cmdline[i]);
                free(cmdline[i]);
                cmdline[i] = checkit;
                printf("%s\n", cmdline[i]);
            }
            if(cmdline[i][a] == '.'){
                for(int b = 0; b < (strlen(cmdline[i]) + 1); b++){
                    if(cmdline[i][b] == '.'){
                    }
                    else {
                        cmdline[i][cmdline_counter_single_dot] = cmdline[i][b];
                        cmdline_counter_single_dot++;
                    }
                }
                Pwd_holder_single_dot = getenv("PWD");
                strcat(Pwd_holder_single_dot, cmdline[i]);
                free(cmdline[i]);
                cmdline[i] = Pwd_holder_single_dot;
                printf("%s", cmdline[i]);
            }
            if(cmdline[i][a] == '~'){
                for(int b = 0; b < (strlen(cmdline[i]) + 1); b++){
                    if(cmdline[i][b] == '~'){
                    }
                    else {
                        cmdline[i][cmdline_tilda] = cmdline[i][b];
                        cmdline_tilda++;
                    }
                }
                Pwd_holder_tilda = getenv("HOME");
                strrev(Pwd_holder_tilda);
                int backslash_checker = 0;
                int checkit_counter = 0;
                for(int b = 0;b < strlen(Pwd_holder_tilda); b++){
                    if(Pwd_holder_tilda[b] == '/'){
                        backslash_checker++;
                    }
                    if(backslash_checker > 0){
                        checkit_tilda[checkit_counter] = Pwd_holder_tilda[b + 1];
                        checkit_counter++;
                    }
                }
                strrev(checkit_tilda);
                strcat(checkit_tilda, cmdline[i]);
                free(cmdline[i]);
                cmdline[i] = checkit_tilda;
                printf("%s\n", cmdline[i]);
            }
        }
    }
    int semi_counter = 0;
    int load_index = 0;
    char* Path_init = (char *)calloc(strlen(getenv("PATH")), sizeof(char *));
    Path_init = getenv("PATH");
//   printf("%s\n", Path_init);
    for(int i = 0; i < strlen(Path_init); i++){
        if(Path_init[i] == ':'){
            semi_counter++;
        }
    }
    int index[semi_counter];
   index[0] = -1;
    for(int i = 1; i < strlen(Path_init); i++){
        if(Path_init[i] ==':'){
            index[load_index]  = i;
            load_index++;
        }
    }
    index[semi_counter] = (strlen(Path_init) - 1);
    for(int i = 0; i < semi_counter + 1;i++){

      //printf("%d\n", index[i]);
    }
    char ** Path_paths = (char **)calloc(semi_counter, sizeof(char **));
    for(int a = 0; a < semi_counter;a++){
        Path_paths[a] = (char *)calloc(50, sizeof(char *));
        if(a == 0){
            strncpy(Path_paths[a], Path_init, index[a]);
        }
      else if (a < semi_counter){
            strncpy(Path_paths[a], Path_init + index[a] + 1, index[a + 1] - index[a] - 1);
       }
    }
    int cmd_index = 0;
    char* command = (char *)calloc(10, sizeof(char *));
    for(int i = 0; i < size;i++){
        char* noback  = strchr(cmdline[i], '/');
        char* noarrowone = strchr(cmdline[i], '<');
        char* noarrowtwo = strchr(cmdline[i], '>');
        char* nopipe = strchr(cmdline[i], '|');
        if (noback == NULL){
          command = cmdline[i];
        cmd_index  = i;
        }
    }
    for(int a = 0; a < semi_counter; a++){
      strcat(Path_paths[a], "/");
      strcat(Path_paths[a], command);
      //printf("%s\n",Path_paths[a]);
    }
    int all_failed = 0;
    for(int a = 0; a < semi_counter; a++){
    FILE* did_open = fopen(Path_paths[a], "r");
    if(did_open != NULL){
      fclose(did_open);
      strcpy(cmdline[cmd_index], Path_paths[a]);
      all_failed++;
      }
    }
    if(all_failed == 0){
      printf("No Match in Path was found, start execution\n");
    }
}

void pipeexe(char **cmdline, int size, int numpipes){
    printf("In pipe function - completely tested with echo but not execution\n");
	  /* parse based on pipelines */
	  int index[numpipes + 2]; // +2 to compensate the begining and end which shouldn't have pipes.
	  int indexcounter = 1;
    // Preset Indexes
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
			      exit(0);
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
        // Parse into 2d arrays to pass to execution function
        int nsize = index[i + 1] - index[i] - 1;
        char ** temp = (char **)calloc(nsize , sizeof(char *));
        int k = 0;
        for(int j = index[i] + 1; j < index[i + 1]; j++){
            temp[k] = (char *)calloc(strlen(cmdline[j]) + 1, sizeof(char));
            temp[k] = cmdline[j];
            k++;
        }

        // Piping
        if(i == 0){ // First Command
            pipe(fd1);
      		  if(pid[i] = fork() == 0){ // Child (cmd1 | cmd2)
                close(STDOUT_FILENO);
                dup(fd1[1]);
                close(fd1[0]);
                close(fd1[1]);

                // Execution
                execution(temp, nsize);
                //echo(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
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
                execution(temp, nsize);
                //echo(cmdline + (1 + index[i]), index[i + 1] - index[i] - 1);
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
        /* Clean-up */
        for(int z = 0; z < nsize; z++)
            free(temp[z]);
        free(temp);
    }
    for(int i = 0; i < numpipes + 1; i++){ // Parent
        waitpid(pid[i], NULL, 0);
    }
    /* end of pipe implementation */
}

void redirection(char **cmdline, int size){
    printf("IN REDIRECTION FUNCTION - Completely tested with echo but not execution\n");
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

    /* Error Handling */
    if(index == 0){ // checks if redirection symbol is at beginning of cmdline
        perror("Error: No command specified");
        exit(0);
    }
    else if(index == size - 1){ // checks if redirection symbol is at end of cmdline
        perror("Error: No file specified");
        exit(0);
    } // file not checked because if file is not in the right place, it wont open

    /* Parse out redirection symbol and filename */
    char ** temp = (char **)calloc(size - 2, sizeof(char *));
    int j = 0;
    for(j = 0; j < index; j++){ // get the cmds and args before the redirection symbol
        temp[j] = (char *)calloc(strlen(cmdline[j]) + 1, sizeof(char));
        temp[j] = cmdline[j];
    }
    for(int i = index + 2; i < size; i++){ // get the args after filename b/c they are considered args too
        temp[j] = (char *)calloc(strlen(cmdline[i]) + 1, sizeof(char));
        temp[j] = cmdline[i];
        j++;
    }

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
        execution(temp, size - 2);
        exit(0);
    }
    else if(pid < 0){
        perror("Fork error in redirection function");
        exit(0);
    }
    else{ // parent process
        close(fd); // Probably not necessary?
        waitpid(pid, NULL, 0);
    }

    /* Memory Clean-up */
    for(int i = 0; i < size - 2; i++)
        free(temp[i]);
    free(temp);
}

void execution(char **cmdline, int size){
    printf("IN EXECUTION FUNCTION - Tested and functional, maybe test when path resolution works\n");
    int pid;
    if(pid = fork() == 0){ // Child process
        /* - List of Execution Processes -
         * Built-ins are checked again despite being checked for in the parsing
         * function. This is mainly for other functions such as pipeexe,
         * redirection, and built-ins.
         */
        if(strcmp(cmdline[0], "exit") == 0)
            B_exit(cmdline, size);
        else if(strcmp(cmdline[0], "/usr/bin/echo") == 0 || strcmp(cmdline[0], "echo") == 0)
            echo(cmdline, size);
        else if(strcmp(cmdline[0], "etime") == 0)
            etime(cmdline, size);
        else if(strcmp(cmdline[0], "io") == 0)
            io(cmdline, size);
        else if(strcmp(cmdline[0], "/usr/bin/cd") == 0 || strcmp(cmdline[0], "cd") == 0)
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
}

/* Built-ins */
int B_exit(char **args, int size){
	  printf("Exiting Shell....\n");
	  exit(0);
}

void cd(char **args, int size){
    printf("IN CD FUNCTION - Need to test completely\n");
    printf("size is %d\n", size);
    for(int i = 0; i < size; i++)
        printf("content: %s\n", args[i]);
	  if(size < 2){ // If no args, $HOME is the arg
        //args[1] = envvar("$HOME"); // Pass through env_var and copy to args[1]
        char* Home = getenv("HOME");
        if(chdir(Home) != 0){
            perror("Error changing directory to $HOME");
            // Signals error if target is not a directory
            B_exit(args, size);
        }
        else{
            if(setenv("PWD", Home, 1) != 0){
                perror("Unable to set $PWD");
            }
	      }
    }
    // Env_var and Path resolution is predetermined already
    else{
        if(chdir(args[1]) != 0){
	          perror("Error");
            // Signals error if target is not a directory
            B_exit(args, size);
        }
        else{
            if(setenv("PWD", args[1], 1) != 0){
                perror("Unable to set $PWD");
            }
        }
    }

}

void echo(char **args, int size){
    printf("In ECHO - Need to test with path resolution and envvar\n");
    //outputs whatever user specifies
	  //for each argument:
		    //look up the argument in the list of environment variables
		    //print the value if it exists
		    //signal an error if it does not exist
    for(int i = 1; i < size; i++){
        printf("%s ", args[i]);
        // env_var and path resolutions are predetermined
        /*if (args[i][0] == '$'){
            // environmental variable
            //printf("HERE is %s\n", envvar(args[i]));
          //  printf("%s ", envvar(args[i]));
            // envvar will print out error message if it does not exist
            i++; // arg is in next index after the $ so increment to skip
        }
        else{
            printf("%s ", args[i]);
        }*/
    }
    printf("\n");
}

void etime(char **args, int size){
    printf("IN ETIME FUNCTION - Fully tested and fully functional\n");
    long sec, usec;
	  struct timeval start, end;
	  gettimeofday(&start, NULL); // get start time

	  // take out "etime" and pass to execute function
    char ** temp = (char **)calloc(size - 1, sizeof(char* *));
    for(int i = 1; i < size; i++){
        temp[i - 1] = (char *)calloc(strlen(args[i]) + 1, sizeof(char));
        temp[i - 1] = args[i];
    }
    execution(temp, size - 1);
    //echo(args + 1, size - 1);
    // passes in by starting from the next command and decrease size to compensate

	  gettimeofday(&end, NULL); // get end time

    // Calculations
    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    if(usec < 0){ // computing for negative microseconds
        sec -= 1;
        usec += 1;
    }
	  printf("Elapsed Time: %ld.%06ld\n", sec, usec);

    /* Clean-up */
    for(int i = 0; i < size - 1; i++){
        free(temp[i]);
    }
    free(temp);
}

void io(char **args, int size){
    printf("IN IO FUNCTION - Fully tested and fully functional\n");
    pid_t pid;
    if(pid = fork() == 0){ // child process
        // take out "io" and pass to execute function
        char ** temp = (char **)calloc(size - 1, sizeof(char* *));
        for(int i = 1; i < size; i++){
            temp[i - 1] = (char *)calloc(strlen(args[i]) + 1, sizeof(char));
            temp[i - 1] = args[i];
        }
        execution(temp, size - 1);
        //echo(args + 1, size - 1);

        // create filepath /proc/<pid>/io
        FILE *fp;
        char file[20] = "\0"; // This assumes pid with at max be 10 digits
        sprintf(file,"/proc/%d/io", getpid());

        // Open the file and read each line
        fp = fopen(file, "r");
        if(fp != NULL){
            char line[50]; // Assuming no more than 50 char per line
            while(fgets(line, 50, fp)){
                // Tokenize each line
                int size = strlen(line);
                int index = strchr(line, ':') - line + 1; // find index of : and index to next char
                char * record = (char *)calloc(index + 2, sizeof(char));
                char * value = (char *)calloc((size + 1) - (index + 1), sizeof(char));
                strncpy(record, line, index + 1);
                strncpy(value, line + index + 1, (size + 1) - (index + 1));

                // Print in tabular format and free dynamic memory
                printf("%-40s%s", record, value);
                free(record);
                free(value);
            }
        }
        else{
            perror("Could not open file in io");
        }

        /* Clean-up */
        for(int i = 0; i < size - 1; i++){
            free(temp[i]);
        }
        free(temp);
        exit(0);
    }
    else if(pid < 0){ // Fork error
        perror("io fork error");
        B_exit(args, size);
    }
    else{ // Parent process
        waitpid(pid, NULL, 0);
    }
}






























































char *strrev(char *str){
    int i = strlen(str) - 1, j = 0;
    char ch;
    while (i > j){
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
 //newpath now contains the file pathway you need for whatever your function
  //you're using it for
  printf("%s", newpath);



  }



  */
