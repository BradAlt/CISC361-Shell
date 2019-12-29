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



int sh( int argc, char **argv, char **envp )
{

  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir, *curdir, *lastdir;
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
  prompt[0] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();
  ogpath = pathlist;

  char buffer[420];
  char* ptr, input, curArg;
  char** userInput;
  char** myArr;
  
  while ( go ) {
    ptr = getcwd(NULL, 0); 
    printf("\n%s <%s> ", prompt, ptr); //Print prompt
    free(ptr);
    strcpy(buffer,"");
    while(strlen(buffer)==0 || buffer[0] == ' '){
      fgets(buffer, 420, stdin); // Get user input
      buffer[strlen(buffer) - 1] = '\0';

      if(strlen(buffer)==0){
        printf("Please input a command: ");
      }
      else if(buffer[0] == ' '){
        printf("Please dont start with a space: ");
      }
      else{
        userInput = stringToArray(buffer, &argsct);
        for(int i = 0; i < argsct; i++){
          //printf("(%i,%s)", i, userInput[i]);
        }
      }
    }

    printf("    ");

    /* get command line and process */
    /* check for each built in command and implement */

    /*  else  program to exec */
       /* find it */
       /* do fork(), execve() and waitpid() */
      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    //----------------------- PATH -----------------------------
    if(strcmp(userInput[0], "path")==0){
      	struct pathelement *tmp = pathlist;
	while(pathlist){
        printf("\n%s", pathlist->element); //Invalid read of size 1
	pathlist = pathlist->next;
      }
	pathlist = tmp;
    }
    //------------------------ WHERE ----------------------------
    else if(strcmp(userInput[0], "where")==0){
      if(strcmp(userInput[1],"")==0){
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
            if(strcmp(userInput[1],"")==0){
        printf("\nWhat to look for: ");
        fgets(buffer, 69, stdin); // Get user input
        buffer[strlen(buffer) - 1] = '\0';
        where(buffer, pathlist);
      }
      else{
        where(userInput[1], pathlist);
      }

    }
     //------------------------ CD ----------------------------
    else if(strcmp(userInput[0], "cd")==0){
      	free(lastdir);
	lastdir = curdir;
      curdir = getcwd(NULL,0);

      if(strcmp(userInput[1], "")==0){
        chdir(homedir);
      }
      else if(userInput[1][0]=='-'){
        printf("Go to previous directory\n");
        chdir(lastdir);
      }
      else{
        ptr = getcwd(NULL, 0);
        char* newDir = malloc(420*sizeof(char));
        strcpy(newDir, ptr);
        strcat(newDir, "/"); 
        strcat(newDir, userInput[1]);
        strcat(newDir, "/"); // go to cwd/input/
        printf("Go to <%s>", newDir);
        chdir(newDir);
	free(ptr);
	free(newDir);
      }
	
      free(curdir);
    }
     //------------------------ PWD ----------------------------
    else if(strcmp(userInput[0], "pwd")==0){
      ptr = getcwd(NULL, 0); 
      printf("Current working directory: <%s> ", ptr); //Print prompt
      free(ptr);
    }
    //------------------------ PRINTENV ------------------------
    //
    else if(strcmp(userInput[0], "printenv")==0){
	if(strcmp(userInput[1], "")==0){
		int cur = 0;
		char* tmp = "true";
		printf("\nEnvironment: ");
		while(tmp){
			tmp = envp[cur];
			printf("\n   %s",tmp);
			cur++;
		}
	}
	else{
		printf("Environment: %s = %s", userInput[1], getenv(userInput[1]));
	}
    }
    //------------------------ SETENV ---------------------------

    else if(strcmp(userInput[0], "setenv")==0){
	if(strcmp(userInput[1],"")==0){
		int cur = 0;
		char* tmp = "true";
		printf("\nEnvironment: ");
		while(tmp){
			tmp = envp[cur];
			printf("\n   %s",tmp);
			cur++;
		}
	}
	else if(strcmp(userInput[1],"PATH")==0){
		free(pathlist->element);
		do {
    			struct pathelement *tmp = pathlist;
    			pathlist = pathlist->next;
    			free(tmp);
  		} while (pathlist);

		
	//Need to convert the remainder of userInput into : delimited string
		char* newPath = malloc(420*sizeof(char));
		strcpy(newPath,"");
		int cur = 2;
		while(strcmp(userInput[cur],"")!=0){
			//printf("Cur newPath: %s", newPath);
			strcat(newPath,"/");
			strcat(newPath, userInput[cur]);
			strcat(newPath,":");
			cur++;
		}
		setenv("PATH", newPath,1);
		free(pathlist);
		pathlist = get_path();
		free(newPath);
	}
	else if(strcmp(userInput[1],"")!=0 && strcmp(userInput[2],"")==0){
		setenv(userInput[1], "", 0);
	}
	else if(strcmp(userInput[1],"")!=0 && strcmp(userInput[2],"")!=0){
		setenv(userInput[1], userInput[2], 1);
	}
	else{
		printf("\nERROR");
	}
    }
     //------------------------ LIST ----------------------------
    else if(strcmp(userInput[0], "list")==0){
      if(strcmp(userInput[1],"")==0){
	char* curThing = getcwd(NULL, 0);
        list(curThing);
	free(curThing);
      }
      else{
        for(int i = 1; i < argsct; i++){
          list(userInput[i]);
        }
      }
    }
     //------------------------ PID ----------------------------
    else if(strcmp(userInput[0], "pid")==0){
      int p_id = getpid();
      printf("Current PID: %i", p_id);
    }
     //------------------------ KILL ----------------------------
    else if(strcmp(userInput[0], "kill")==0){
      
      if(userInput[1][0] == '-'){
        char* tmp = malloc(69*sizeof(char));
        for(int i = 1; i < strlen(userInput[1]); i++){ //Kinda gross
          tmp[i-1] = userInput[1][i]; //Adds everything from the user input, except the '-'
        }
        kill(atoi(userInput[2]),atoi(tmp));
        free(tmp);
      }
      else{
        kill(atoi(userInput[1]), 9); //I mean, I guess 9 is the go-to kill thing right?
      }
    }
     //------------------------ PROMPT ----------------------------
    else if(strcmp(userInput[0], "prompt")==0){
      strcpy(prompt,userInput[1]);
    }
    //------------------------ EXIT-----------------------
    else if(strcmp(userInput[0], "exit") == 0){
      go = 0;
    }
     //------------------------- ELSE ------------------------
    else
    {
      //------------------ Execute program --------------
      pid_t pid;

      if ((pid = fork()) < 0) {
        puts("ERROR");
      }
      else if (pid == 0) {
        char* myProgram = getcwd(NULL, 0); 
        strcat(myProgram,"/");
        strcat(myProgram,userInput[0]);
        execve(myProgram, argv, envp);
        free(myProgram);
        return 0;
        
      }
      else {
        waitpid(pid, NULL, 0);
      }

    }





    //printf("\nFreeing: ");
    for(int i = 0; i < argsct+1; i++){
      //printf("(%i,%s)",i ,userInput[i]);
      free(userInput[i]);
    }
    free(userInput);
  }
  //printf("\nOUT OF WHILE GO LOOP");
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

void list(char *dir)
{
  printf("Listing contents of: %s", dir);
  DIR *myDir = opendir(dir);
  if (myDir) { //If successfully opened
    struct dirent *curEntry;
    while (curEntry = readdir(myDir))
    {
      printf("\n  ~ %s",curEntry->d_name);
    }
    printf("\n");
    closedir(myDir);
  }

  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */

