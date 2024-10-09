
/**********************************************
 * -program by V.Lee, Nov.14, 1996
 * -no dynamic allocation needed
 * -to build a tree of comdecks for each program
 **********************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#define NCARMAX 128
#define ELEMMAX 30000
 
int element_max;
int element_min;
struct arbre
{
     char *parent;
     char *child;
     struct arbre *sis;
};
struct arbre node[ELEMMAX];
char elem[ELEMMAX][NCARMAX];
FILE *liste,*dependance;
int binary(char *name);


/*   program to construct a tree of dependencies */

int main(int argc, char **argv)
{
    void tree();

    char *cpointeur;
    int i,j,k,pflag;


/*  open input file */

        argv++;
        if ((liste = fopen(*argv,"r")) == (FILE *) NULL)
        {
            fprintf(stderr,"on ne peut ouvrir le fichier %s\n",*argv);
            exit(0);
        }

/*   open output file arbre_de_dependances   */

        if ((dependance = fopen("arbre_de_dependance","w")) == (FILE *) NULL)
        {
            fprintf(stderr,"on ne peut ouvrir le fichier %s\n","arbre_de_dependances");
            exit (0);
        }

/*    To build the array of elements with dependencies.
      Each "element" contains a parent and a child */

        k=0; /* counter for pairs of parent and child */
        i=0; /* counter from top of node array */
        j=ELEMMAX-1; /* counter from bottom of node array */

        while(fgets(elem[k],NCARMAX,liste) != (char *) NULL)
        {
           cpointeur= strchr(elem[k],' ');
           *cpointeur = '\0';
        /* if parent is already at the top part of the node array,
           put element at the bottom part of the node array*/
           if (k>0 && strcmp(elem[k],elem[k-1])==0){
               node[j].parent = elem[k];
               node[j].child  = cpointeur+1;
             /* set sister of the last element to point to
                current element */
               if (j<ELEMMAX-1 && strcmp(node[j+1].parent,elem[k])==0){
                   node[j+1].sis = &node[j]; 
                   }
               else{
               node[i-1].sis = &node[j];
                }
             /* set sister of current element to point to NULL */
               node[j].sis = NULL;
               cpointeur= strchr(node[j].child,'\n');
               *cpointeur = '\0';
               j--;
               }
           else{
         /* new parent found, put in top part of node array*/
               node[i].parent = elem[k];
               node[i].child = cpointeur+1;
               node[i].sis = NULL;
               cpointeur= strchr(node[i].child,'\n');
               *cpointeur = '\0';
               i++;
               }
           k++;
           if (k > ELEMMAX || i==j){
               fprintf(stderr,"NUMBER OF ELEMENTS (pairs) exceed %d\n",ELEMMAX);
               exit(1);
               }
        }
         element_min = j;
         element_max = i;
     /*  printf("max nodes to search for a child who is a parent is %d\n",
         element_max); */

     /* loop to print out to file "arbre_de_dependance"
        and to file stdout "_liste_tempor" */
         for (i=0;i<element_max;i++){
           pflag = 0; /* print flag to file "arbre_de_dependance"*/
           if (strstr(node[i].parent,".cdk") == NULL &&
               strstr(node[i].parent,".h")   == NULL){
               pflag = 1;
               fprintf(dependance,"%s\n",node[i].parent);
               }
           tree(node[i],node[i].parent,pflag,1);
           if (pflag) fprintf(dependance,"\n");
           }

         fclose(liste);
         fclose(dependance);
}

void tree(pointer,name,pflag,level)
/* Routine to create tree of comdecks.
   pointer: pointer to current node 
   name:    name of current node
   pflag:   flag=1 to print to file arbre_de_dependance
   level:   level of tree depth
*/
   
struct arbre pointer;
char *name[];
int pflag,level;
{

struct arbre npointer;
   int i,j,k;

    if (level > 40) {
        fprintf(stderr,"ERROR, tree dependance more than 40 levels\n");
        fprintf(stderr,"Check to see if there is a infinite dependance\n");
        exit(1);
        }
   printf("%s %s\n",name,pointer.child);
   printf("%s %s\n",pointer.child,name);
   if (pflag){
   for(i=0;i<10*level;i++) fprintf(dependance," ");
   fprintf(dependance,"%s\n",pointer.child); 
   }

   /* binary search through head nodes */
   j = binary(pointer.child);
   if (j>=0){
      tree(node[j],name,pflag,level+1);
      }
   if (pointer.sis != NULL){
       npointer = *pointer.sis;
       tree(npointer,name,pflag,level);
       }
}

int binary(char *name)
/* algorithm from "The C programming language" */
{
   int i,j,k;

   i=0;
   k=element_max-1;
   while(i<=k) {
       j= (i+k)/2;
       if (strcmp(name,node[j].parent) <0) 
              k=j-1;
       else if (strcmp(name,node[j].parent)>0)
              i=j+1;
       else return (j);
       }
       return(-1);
}
