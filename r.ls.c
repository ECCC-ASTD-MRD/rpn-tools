#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
/*
#include <netdb.h>
extern int h_errno;
*/

#define is_a_dir(a) ((a>>12)==4)
#define is_a_file(a) ((a>>12)==010)
#define is_a_link(a) ((a>>12)==012)

main(int argc , char **argv)
{
struct stat stat_1;
struct stat lstat_1;
struct dirent *entry;
DIR *directory;
int val1,val2,i;
char linkname[1024], buffer[10240];
char *delimiter;
char *remhost;
char *temp, *progname;
char hostname[1024];
int remotecall=0;
/*
struct hostent *host_entry;
*/

/* get program name for rsh call */

progname=argv[0];
temp=argv[0];
while (*temp) {
   if (*temp=='/')
      progname=temp+1;
      temp++;
   }

/* get host name, use it if remote call    */
/* if rsh, env variable ARMNLIB is unknown */

gethostname(hostname,1022);
if(strcmp("mach",hostname)==0) strcpy(hostname,"maxwell");
i=strlen(hostname);
hostname[i]=':';
hostname[i+1]='\0';
if( NULL != getenv("ARMNLIB") ) hostname[0]='\0';
remhost=hostname;

if ( argc < 2 ) {
   printf("USAGE: r.ls [machine:]directory\n");
   exit(1);
   }

/* check for remote reference machine:path */
/* if so issue an rsh to remote machine    */

if ( delimiter=strstr(argv[1],":" ) ) {
   *delimiter=0;
   sprintf(buffer,"rsh %s /usr/local/env/armnlib/bin/%s %s",argv[1]
           ,progname,delimiter+1);
   system(buffer);
   exit(0);
   }

stat(argv[1],&stat_1);
val1=stat_1.st_mode;
if ( ! is_a_dir(val1) ) {              /* argument 1 MUST be a directory */
   printf("ERROR NOT_A_DIRECTORY\n");
   exit(1);
   }
if ( NULL == (directory=opendir(argv[1])) ) {
   printf("ERROR CANT_OPEN_DIRECTORY\n");
   exit(1);
   }
if ( chdir(argv[1]) ) {               /* MUST be readable and chdir able */
   printf("ERROR CANT_CHDIR\n");
   exit(1);
   }

while (entry=readdir(directory)){     /* pass 1, directories and remotes */
   if(entry->d_name[0] != '.') {      /* do not process "hidden" entries */
      if ( stat(entry->d_name,&stat_1) ) val1=0120000 ;
      else                               val1=stat_1.st_mode;
      if (lstat(entry->d_name,&lstat_1)) val2=0 ;
      else                               val2=lstat_1.st_mode;

      if ( is_a_dir(val1) ){          /* directory, list in pass 1  */
         if(NULL!=strstr(argv[1]," "))  continue;
         printf("%s/ %s%s/%s 0\n",entry->d_name,remhost,argv[1],entry->d_name);
         continue;
         }

      if ( is_a_link(val2) && is_a_link(val1) ){  /* list "special" links in pass 1 */
         for(i=0;i<1023;i++)linkname[i]=0;
         readlink(entry->d_name, linkname,1023);
         if ( NULL == (delimiter=strstr(linkname,":" )) ) continue;
         if(NULL!=strstr(linkname," ")) continue;
/*
         *delimiter=0;
         host_entry=gethostbyname(linkname);
         printf("Address of %s is %x\n",linkname,host_entry->h_addr_list[0]);
         *delimiter=':';
*/
         printf("%s@ %s 0\n",entry->d_name,linkname);
         continue;
         }
      }
}

rewinddir(directory);                  /* pass2, list ordinary files */
while (entry=readdir(directory)){
   if(entry->d_name[0] != '.') {
      if ( stat(entry->d_name,&stat_1) ) val1=0120000 ;
      else                               val1=stat_1.st_mode;
      if ( is_a_file(val1) ){
         if(NULL!=strstr(entry->d_name," ")) continue;
         printf("%s %s%s/%s %d\n",entry->d_name,remhost,argv[1],entry->d_name
                 ,stat_1.st_size>>10);
         }
      }
   }

closedir(directory);
}
