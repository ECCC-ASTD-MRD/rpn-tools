
/*************************************************************************
 *   programme servant a comprimer une page de texte provenant           *
 *   d'un man page si elle comporte plus de 60 lignes                    *
 *                                                                       *
 *   on elimine les lignes vides du debut et de la fin et si necessaire, *
 *   les lignes vides a l'interieur du texte.  si on ne peut rien        *
 *   eliminer, on imprime la page telle quel.                            *
 *                                                                       *
 *   auteur: J. Caveen, RPN, aout 1991                                   *
 *                                                                       *
 *************************************************************************/

#include <stdio.h>

#define LMAX   60
#define LMAXM1 59
#define MAXD   80
#define MAXDM1 79
#define MAXCAR 80


main(argc,argv)
int argc;
char *argv[];

{
     FILE *impair, *pair;

    void compression(), ecrit_la_page();
    char page[MAXD][MAXCAR];
    char *ligne[MAXD];
    int nbligne = -1, i, page_num = 0;

/*
 *  ouverture du ou des fichiers de sortie
 */
    
    
    argv++;
    if((impair = fopen (*argv, "a")) == (FILE *) NULL)
    {
       fprintf(stderr," On ne peut ouvrir le fichier %s\n",*argv);
       exit(1);
    }
    if(argc == 3)
    {
       argv++;
       if((pair = fopen (*argv, "a")) == (FILE *) NULL)
       {
          fprintf(stderr," On ne peut ouvrir le fichier %s\n",*argv);
          exit(1);
       }
    }
    else
    {
        pair=impair;
    }

/*
 *  initialiser les pointeurs de lignes
 */

    for(i=0; i< MAXD; i++)
         ligne[i] = page[i] ;

    while((gets(ligne[++nbligne]) != (char *) NULL) )
    {
       if((ligne[nbligne][0] == '\f') || (nbligne == MAXDM1))
       {
            page_num++;
/*
 *          on a rencontre une fin de page: on la traite
 */
            compression(ligne,nbligne);
            if(page_num % 2)
                ecrit_la_page(ligne,nbligne,impair);
            else
                ecrit_la_page(ligne,nbligne,pair);
            nbligne = -1;
       }

    }

/*
 *  on traite la derniere page   
 */
    nbligne--;
    compression(ligne,nbligne);
    if(page_num % 2)
         ecrit_la_page(ligne,nbligne,impair);
    else
         ecrit_la_page(ligne,nbligne,pair);

/*
 *  fermeture du ou des fichiers
 */
    fclose(impair);
    if(argc == 3)
        fclose(pair);

}


/******************************************************************
 * fonction servant a enlever les lignes vides a l'interieur      *
 * d'un texte.  on enleve d'abord les lignes du debut             *
 * ensuite celles de la fin et finalement les lignes a l'interieur*
 * du texte.  on elimine les lignes jusqu'a ce qu'il n'en reste   *
 * plus que 60.                                                   *
 ******************************************************************/
void compression(ligne,nbligne)
char *ligne[];
int nbligne;
{
      int i, nombre; 

/*
 *     verifier si on a 60 lignes ou moins
 */
      if(nbligne <= LMAXM1)
           ;

      else
      {
/*
 *      on enleve les lignes vides du debut
*/
        nombre = nbligne;
        i = 0;
        while((strlen(ligne[i]) == 0) && (nombre > LMAXM1))
        {
           strcpy(ligne[i],"bbiiddoon"); 
           nombre--; 
           i++;
        }

/*
 *      on enleve les lignes vides de la fin
*/
        i = nbligne - 1;
        while((strlen(ligne[i]) == 0) && (nombre > LMAXM1))
        {
         strcpy(ligne[i],"bbiiddoon"); 
           nombre--;
           i--;
         }
/*
 *       on enleve les lignes vides a partir du debut du texte
 */

         i = 0;
         while((i < nbligne) && (nombre > LMAXM1))
         {
             if(strlen(ligne[i]) == 0)
             {
                strcpy(ligne[i],"bbiiddoon"); 
                nombre--;
             }
             i++;
          }
                   
       }
}


/*****************************************************************
 * fonction ecrivant sur fichier toutes les lignes qui ne sont   *
 * egales a la chaine de controle "bbiiddoon"                    *
 *****************************************************************/
void ecrit_la_page(ligne,nbligne,fichier)
char *ligne[];
int nbligne;
FILE *fichier;
{
   
     int i = 0;
     
     for (i = 0; i<=nbligne-1; i++)
     {
         if(strcmp(ligne[i],"bbiiddoon") != 0) 
         {
             fputs(ligne[i],fichier);
             fputc('\n',fichier);
         }
     }
     fputc('\f',fichier);
}
