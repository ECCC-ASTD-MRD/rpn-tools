#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NCARMAX 256
#define NOMMAX 128

/************************************************************
 * program to establish the list of dependencies for each   *
 * given file by extracting all names in the "include"      *
 * option "-q" greps only files surrounded by "quotes"      *
 *                                                          *
 * usage:  echo *.ftn | grepdep -q > liste_include          *
 *         echo *.c | grepdep > liste_include               *
 *                                                          *
 * original version by James Caveen, November 1991          *
 * modified by V.Lee, November 1996                         *
 ************************************************************/
int main(int argc, char *argv[])
{

    char ligne[NCARMAX], nom[NOMMAX], comdeck[NOMMAX], *pointeur;
    int c,i, on_continue=0,quotes;
    FILE *fichier;
    void get_comdeck();

/*  By default, grepdep will comdeck names surrounded by quotes
    and "<" and ">" signs */

    quotes=0;
    if (argc>1) {
       if(strcmp(argv[1],"-q")==0) quotes=1;
       else quotes=0;
       }

/* extract filename from stdin list */
    c = getchar();
    if(c != '\n')
        on_continue=1;
    while(on_continue)
    {
        i=0;
        while((c != ' ') && (c != '\n'))
        {
          nom[i++] = c;
          c = getchar();
        }
        nom[i]='\0';

        if ((fichier = fopen(nom,"r")) == (FILE *) NULL)
        {
            fprintf(stderr,"on ne peut ouvrir le fichier %s\n",nom);
        }

/* after opening the file, eliminate the RCS extension ",v"
   on the name of the file */

        pointeur = strchr(nom,',');
        if (pointeur != (char *) NULL) *pointeur = '\0';


        while(fgets(ligne,NCARMAX,fichier) != (char *) NULL)
        {
            if((ligne[0] == '#') || ((ligne[0] == '@') && (ligne[1] == '#')))
            {
                  pointeur= strchr(ligne,'#');
                  if(strstr(pointeur,"include")!=NULL)
                  {
                     pointeur= strchr(ligne,'"');
                     if (pointeur == (char *) NULL && !quotes)
                            pointeur= strchr(ligne,'<');
                     if (pointeur != (char *) NULL){
                     comdeck[0]='\0';
                     get_comdeck(pointeur+1,comdeck);
                     fprintf(stdout,"%s %s\n",nom,comdeck);
                     }
                  }
            }
                   
        }
        fclose(fichier);
        if(c == '\n')
           on_continue=0;
        c = getchar();
     }
}        


void get_comdeck(buf,nom)
char *buf, *nom;
/* from buf, get the comdeck name */

{
     while(*buf)
     {
       if(*buf == '\n'|| *buf=='"' || *buf=='>')
          break;

       *nom++ = *buf++;
     }

     *nom = '\0';
}

