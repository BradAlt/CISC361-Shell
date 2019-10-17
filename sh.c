#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

char** stringToArray(char* input, int* myArgsCount){
    char buf[69];
    strcpy(buf, input);
    char* t = strtok(buf," ");
    int cur = 1;
    while(strtok(NULL," ")){
        cur++;
    }
    char** myArgs = malloc((cur+1) * sizeof(char*)); //Need space for however big each strtok chunk is (and each is a char array) plus the terminating char
    //We might as well fill in that \0 now
    myArgs[cur] = malloc(2*sizeof(char));
    strcpy(buf, input);
    strcpy(myArgs[cur], "\0");
    //Not actually have to fill in each piece of args
    cur = 0;
    t = strtok(buf," ");
    while(t){
        int len = strlen(t);
        myArgs[cur] = malloc((len+1)*sizeof(char));
        strcpy(myArgs[cur],t);
        //printf("Filled position %i with %s\n", cur, t);
        cur++;
        *myArgsCount = cur;
        t = strtok(NULL," ");
        //printf("New: %s\n", t);
    }
    return myArgs;
}

int sh( int argc, char **argv, char **envp )
{

  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist, *ogpath;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start out with*/
 
  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();
  ogpath = pathlist;

  char buffer[69];
  char* ptr, input, curArg;
  char** userInput;
  char** myArr;

  while ( go ) {
    ptr = getcwd(NULL, 0); 
    printf("\n<%s> ", ptr); //Print prompt
    free(ptr);

    strcpy(buffer,"");
    while(strlen(buffer)==0 || buffer[0] == ' '){
      fgets(buffer, 69, stdin); // Get user input
      buffer[strlen(buffer) - 1] = '\0';

      if(strlen(buffer)==0){
        printf("Please input a command: ");
      }
      else if(buffer[0] == ' '){
        printf("Please dont start with a space: ");
      }
      else{
        userInput = stringToArray(buffer, &argsct);
      }
    }


    /* get command line and process */
    /* check for each built in command and implement */

    /*  else  program to exec */
       /* find it */
       /* do fork(), execve() and waitpid() */
      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    //----------------------- PATH -----------------------------
    if(strcmp(userInput[0], "path")==0){
      while(pathlist->next){
        pathlist = pathlist->next;
        printf("%s\n", pathlist->element); //Invalid read of size 1
      }
    }
    //------------------------ WHERE ----------------------------
    else if(strcmp(userInput[0], "where")==0){
      if(userInput[1]){
        printf("\nWhat to look for: ");
        fgets(buffer, 69, stdin); // Get user input
        buffer[strlen(buffer) - 1] = '\0';
        where(buffer, pathlist);
      }
      else{
        where(userInput[1], pathlist);
      }
    }
     //------------------------ WHICH --------------------------
    else if(strcmp(userInput[0], "which")==0){
      printf("Placeholder for %s", userInput[0]);
    }
     //------------------------ CD ----------------------------
    else if(strcmp(userInput[0], "cd")==0){
      if(strcmp(userInput[1], "")==0){
        printf("\nGo to home directory! PLACEHOLDER");
        chdir("/usa/brada/");
      }
      else if(userInput[1][0]=='-'){
        printf("Go to previous directory\n");
      }
      else if(userInput[1][0]=='*'){
        chdir("~");
      }
      else{
        ptr = getcwd(NULL, 0);
        char* newDir = malloc(69*sizeof(char));
        strcpy(newDir, ptr);
        strcat(newDir, "/"); 
        strcat(newDir, userInput[1]);
        strcat(newDir, "/");
        printf("Go to <%s>", newDir);
        chdir(newDir);

      }
      
    }
     //------------------------ PWD ----------------------------
    else if(strcmp(userInput[0], "pwd")==0){
      ptr = getcwd(NULL, 0); 
      printf("Current working directory: <%s> ", ptr); //Print prompt
      free(ptr);
    }
     //------------------------ LIST ----------------------------
    else if(strcmp(userInput[0], "list")==0){
      printf("Placeholder for %s", userInput[0]);
    }
     //------------------------ PID ----------------------------
    else if(strcmp(userInput[0], "pid")==0){
      printf("Placeholder for %s", userInput[0]);
    }
     //------------------------ KILL ----------------------------
    else if(strcmp(userInput[0], "kill")==0){
      printf("Placeholder for %s", userInput[0]);
    }
     //------------------------ PROMPT ----------------------------
    else if(strcmp(userInput[0], "prompt")==0){
      printf("Placeholder for %s", userInput[0]);
    }
    //------------------------ EXIT-----------------------
    else if(strcmp(userInput[0], "exit") == 0){
      go = 0;
    }
    else{
      printf("%s: Command not found", userInput[0]);
    }
  
    for(int i = 0; i < argsct+1; i++){
      free(userInput[i]);
    }
    free(userInput);
    printf("\n");
  }

  // Seemingly doesnt actually fix any leaks
  // pathlist = get_path();

  free(pathlist->element);
  do {  
    struct pathelement *tmp = pathlist;
    pathlist = pathlist->next;
    free(tmp);
  } while (pathlist);

  free(pwd);
  free(prompt);
  free(commandline);
  free(args);
  free(owd);
  
  return 0;
} /* sh() */


char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  char *cmd = malloc(1024 * sizeof(char*));
  struct pathelement *tempPath = pathlist;
  strcpy(cmd, command);
  while(pathlist){
    if(command != NULL){
      sprintf(cmd,"%s/%s", tempPath->element, command);
      if(access(cmd, F_OK) == 0){
        printf("%s\n", cmd);
        char* temp = cmd;
        free(cmd);
        return temp;
      }
    }
  tempPath = tempPath->next;
  }
  return NULL;
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */

