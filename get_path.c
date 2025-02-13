/*
  get_path.c
  Ben Miller

  Just a little sample function that gets the PATH env var, parses it and
  puts "components" into a linked list, which is returned.
*/
#include "get_path.h"

struct pathelement *get_path()
{
  /* path is a copy of the PATH and p is a temp pointer */
  char *path, *p;

  /* tmp is a temp point used to create a linked list and pathlist is a
     pointer to the head of the list */
  struct pathelement *tmp, *pathlist = NULL;

  p = getenv("PATH");	/* get a pointer to the PATH env var.
			   make a copy of it, since strtok modifies the
			   string that it is working with... */
  path = malloc((strlen(p)+1)*sizeof(char));	/* use malloc(3) */
  strncpy(path, p, strlen(p));
  path[strlen(p)] = '\0';

  p = strtok(path, ":"); 	/* PATH is : delimited */
  //printf("Get path debug\n");
  do				/* loop through the PATH */
  {				/* to build a linked list of dirs */
    
    if ( !pathlist )		/* create head of list */
    {
      tmp = calloc(1, sizeof(struct pathelement));
      pathlist = tmp;
    }
    else			/* add on next element */
    {
      tmp->next = calloc(1, sizeof(struct pathelement));
      tmp = tmp->next;
    }
    //printf("Adding to LL: %s\n", p);
    tmp->element = p;	
    tmp->next = NULL;
  } while ( p = strtok(NULL, ":") );


  //free(path);
  //free(p);
  return pathlist;
} /* end get_path() */
