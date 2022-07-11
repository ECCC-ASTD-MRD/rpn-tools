#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

main(int argc, char **argv)
{
  pid_t *pid_list;
  int i;
  
  if (argc <= 1) {
    printf("process_watch error: invalid number of arguments\n");
    exit(1);
    }
    
  pid_list = (pid_t *) malloc((argc-1)*sizeof(pid_t));
  
  for (i=0; i<argc-1; i++) {
    pid_list[i] = atoi(argv[i+1]);
/*    printf("Debug pid_list[%d]=%d\n",i,pid_list[i]); */
    }
  
/*  pid_t pid=atoi(argv[1]);
  if(kill(pid,0)) exit(1);
*/
  while (1)
    {
      for (i=0; i<argc-1; i++)
        if (kill(pid_list[i],0)) exit(0);
      sleep(3);
    }  
  exit(0);
}
