
/*****************************************************************************
 *  programme servant a transformer un fichier texte                         *
 *   de majuscule a minuscule.                                               *
 *   on indique le traitement a faire aux chaine de type citation            *
 *   grace a l'argument -in et le traitement des chaines ordinaires          *
 *   avec l'argument -ex.                                                    *
 *   pour le traitement des commentaires, on utilise la clef -com            *
 *   valeurs possibles pour -com,-in et -ex:                                 *
 *             min    -  traduire a minuscule                                *
 *             maj    -  traduire a majuscule                                *
 *             tel    -  laisser tel quel                                    *
 *                                                                           *
 *   une directive de cpp demeure  intouchee                                 *
 *                                                                           *
 *   traffic -h retourne  la sequence d'appel                                *
 *                                                                           *
 *   Auteur: James Caveen mai 1991                                           *
 *                                                                           *
 *   Modification: avril 1992 - taitement des commentaires                   *
 *                            - impression de la sequence d'appel            *
 *                            - ajout de la fonction seq_appel               *
 *                            - ajout de la fonction traitement              *
 *                                                                           *
 *****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ncarmax 132
#define ncarplus 133
#define nommax 133

void reduit(), augmente(), telquel();
int seq_appel();


main(argc,argv)
int argc;
char *argv[];

{
     FILE *entree;
     void (*alin)(), (*alex)(), (*commen)(); /*pointeurs aux fonctions a
                                               utiliser pour -in , -ex
                                               et -com */
     void (*(traitement()))();

     int ligne, i;

     char buffer[ncarplus];

     if( argc < 2)
     {
        fprintf(stderr, " On doit fournir le nom du fichiers\n");
        exit(seq_appel(1));
     }
     else if ((argc == 2) && (!strcmp(argv[1],"-h\0")))
     {
        exit(seq_appel(0));
     }

/*   ouverture du fichier d'entree  */
     if( (entree = fopen (argv[1], "r")) == (FILE *) NULL)
     {
        fprintf(stderr, "On ne peut lire sur %s \n",argv[1]);
        exit(1);
     }

/*
     determiner le traitement pour les chaines a l'interieur et a l'exterieur
     des apostrophes/guillemets et pour les commentaires
*/
     alin = telquel ; alex = reduit; commen = reduit;
     if(argc > 2)
     {
           i = 2;
           while(argv[i])
           {
               if(!strcmp(argv[i],"-in\0"))
               {
                  alin = traitement(argv[i+1]);
               }
               else if (!strcmp(argv[i],"-ex\0"))
               {
                  alex = traitement(argv[i+1]);
               }
               else if (!strcmp(argv[i],"-com\0"))
               {
                  commen = traitement(argv[i+1]);
               }
               else
               {
                  fprintf(stderr," Nom de clef non valide :%s\n",argv[i]);
                  exit(seq_appel(1));
               }
                i +=2;
              }
         }
/*   boucle sur toutes les lignes du fichier d'entree  */

     while((ligne = fgets(buffer,ncarmax,entree) != NULL))
     {

          if(*buffer == '#')
              ;
          else if ((*buffer == 'C') || (*buffer == 'c') || (*buffer == '*'))
          {
             (*commen)(buffer,commen,0,buffer);
          }
          else
          {
             (*alex)(buffer,alin,0,buffer);
          }
              printf("%s",buffer);
              buffer[0] = '\0';
     }

/*   fermeture des fichiers 
*/

     fclose(entree);

     exit(0);

}


/**********************************************************
 *                traduire en minuscules                  *
 *   Auteur: James Caveen mai 1991                        *
 *                                                        *
 **********************************************************/


void reduit(buf,fonction,indice,apos1)
char *buf,*apos1;
void (*fonction)();
int indice;

/* cette fonction traduit un chaine de caractere en minuscules
*  on fait la traduction a partir de *buf jusqu'a apos1
*
*  arguments
*         buf       -  vecteur contenant la chaine a traduire
*         fonction  -  pointeur a la fonction qui fera le traitement
*                      a partir de la position apos1
*         indice    -  indice  = 0 traitement d'une chaine ordinaire
*                      indice  = 1 traitement d'une chaine citation
*         apos1     -  pointeur a la position ou fini le traitement
*/
{
     char *apos2;
     char *mygetdelim();
     int tolower();

     if(indice == 0)
     {
        apos1=mygetdelim(buf);
        indice =  1;
     }
     else
     {
         indice = 0;

     }

     if(apos1 == (char *) NULL)
     {
        while(*buf)
        {
            *buf = tolower(*buf);
            buf++;
        }
     }
     else
     {
        while(buf < apos1)
        {
           *buf = tolower(*buf);
           buf++;
        }
        apos2 = strchr(apos1+1,*apos1);
        (*fonction)(apos1+1,reduit,indice,apos2);
      }
}


/**********************************************************
 *              traduire en majuscules                    *
 *   Auteur: James Caveen mai 1991                        *
 **********************************************************/

void augmente(buf,fonction,indice,apos1)
char *buf,*apos1;
void (*fonction)();
int indice;

/* cette fonction traduit un chaine de caractere en majuscules
*  on fait la traduction a partir de *buf jusqu'a apos1
*
*  arguments
*         buf       -  vecteur contenant la chaine a traduire
*         fonction  -  pointeur a la fonction qui fera le traitement
*                      a partir de la position apos1
*         indice    -  indice  = 0 traitement d'une chaine ordinaire
*                      indice  = 1 traitement d'une chaine citation
*         apos1     -  pointeur a la position ou fini le traitement
*/
{
     char *apos2;
     int toupper();
     char *mygetdelim();

     if(indice == 0)
     {
         apos1 = mygetdelim(buf);
         indice = 1;
     }
     else
     {
          indice = 0;
     }
     if(apos1 == (char *) NULL)
     {
        while(*buf)
        {
            *buf = toupper(*buf);
            buf++;
        }
     }
     else
     {
        while(buf < apos1)
        {
           *buf = toupper(*buf);
           buf++;
        }
        apos2 = strchr(apos1+1,*apos1);
        (*fonction)(apos1+1,augmente,indice,apos2);
      }
}


/*************************************************************
 *              laisser les caracteres tels quel             *
 *   Auteur: James Caveen mai 1991                           *
 *************************************************************/

void telquel(buf,fonction,indice,apos1)
char *buf,*apos1;
void (*fonction)();
int indice;

/* cette fonction ne fait aucun traitement a la chaine
*  on laisse la chaine telle quel a partir de *buf jusqu'a apos1
*
*  arguments
*         buf       -  vecteur contenant la chaine a traduire
*         fonction  -  pointeur a la fonction qui fera le traitement
*                      a partir de la position apos1
*         indice    -  indice  = 0 traitement d'une chaine ordinaire
*                      indice  = 1 traitement d'une chaine citation
*         apos1     -  pointeur a la position ou fini le traitement
*/
{
     char *apos2;
     int toupper();
     char *mygetdelim();

     if(indice == 0)
     {
         apos1 = mygetdelim(buf);
         indice = 1;
     }
     else
     {
         indice = 0;
     }
     if(apos1 == (char *) NULL)
     {
          ;
     }
     else
     {
        apos2 = strchr(apos1+1,*apos1);
        (*fonction)(apos1+1,telquel,indice,apos2);
      }
}



/**********************************************************************
 *           chercher le premier delimiteur de chaine                 *
 *   Auteur: James Caveen mai 1991                                    *
 **********************************************************************/

char *mygetdelim(buffer)
char *buffer;

/*     fonction servant a trouver le premier delimiteur a partir de la 
*      position *buf.  un delimiteur est soit un apostrophe ou un guillemet
*      la fonction retourne le pointeur au caractere delimiteur.
*      si on ne trouve aucun delimiteur, on retourne le pointeur (char *) NULL
*
*  argument
*        buffer   -    chaine de caracteres a convertir
*/
{
             char *apos, *guil;

             apos = strchr(buffer,'\'');
             guil = strchr(buffer,'"');
             if((apos == (char *) NULL) && (guil != (char *) NULL))
             {
                 return(guil);
             } 
             else if((guil == (char *) NULL) && (apos != (char *) NULL ))
             {
                 return(apos);
             }
             else if((guil == (char *) NULL) && (apos == (char *) NULL))
             {
                 return((char *) NULL);
             }
             else
             {
                 return((apos > guil) ? guil : apos);
             }
}



/*************************************************************************
 * fonction retournant la sequence d'appel sur demande ou en cas d'erreur*
 *   Auteur: James Caveen avril 1992                                     *
 *                                                                       *
 *************************************************************************/

int seq_appel(val_exit)
int val_exit;

{
    fprintf(stderr,"\n\n\n");
    fprintf(stderr," traffic - sequence d'appel:\n\n");
    fprintf(stderr," traffic fichier [-in  traitement]\n");
    fprintf(stderr,"                 [-ex  traitement]\n");
    fprintf(stderr,"                 [-com traitement]\n\n");
    fprintf(stderr," Ou 'traitement' peut prendre une des valeurs suivantes\n");
    fprintf(stderr,"      maj - conversion a des majuscules\n");
    fprintf(stderr,"      min - conversion a des minuscules\n");
    fprintf(stderr,"      tel - aucune conversion\n\n");
    fprintf(stderr," Pour obtenir la sequence d'appel, tapez:\n\n");
    fprintf(stderr,"     traffic -h\n");
   
    return(val_exit);
}



/**************************************************************************
 * fonction servant a determiner le traitement a faire aux chaines        *
 *                                                                        *
 * traitement retourne un pointeur a une fonction retournant un type void *
 *   Auteur: James Caveen avril 1992                                      *
 *                                                                        *
 **************************************************************************/
void (*(traitement(argv)))()
char *argv;
{
      
      if(!strcmp(argv,"maj\0"))
      {
           return(augmente);
      }
      else if(!strcmp(argv,"min\0"))
      {
           return(reduit); 
      }
      else if (!strcmp(argv,"tel\0"))
      {
           return(telquel);
      }
      else
      {
           fprintf(stderr," Valeur de clef non valide: %s\n", argv);
           exit(seq_appel(1));
      }
}
