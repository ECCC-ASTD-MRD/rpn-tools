#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NCARMAX 40

int static match = 1;
char *position;

int main(int argc, char *argv[])
{
    char *verifie();
    char *trouve_pos();

    char chain[NCARMAX], pat[NCARMAX];
    char *chaine, *patron, *chaine1, *patron1;
    int longueur;
    int retourne_extention = 0;

    if(argc < 3)
    {
       fprintf(stderr," \nusage:\n       r.basename [-e] filename 'ext'\n");
       exit(1);
    }
    
    chaine1 = &chain[1];
    patron1 = &pat[1];
    *(chaine1-1)='\0';
    *(patron1-1)='\0';
    if(argc == 4)
    {
        retourne_extention=1;
        strcpy(chaine1,argv[2]);
        strcpy(patron1,argv[3]);
        longueur = strlen(argv[2]);
        chaine = &chain[longueur];
        longueur = strlen(argv[3]);
        patron = &pat[longueur];
     }
     else
     {
        strcpy(chaine1,argv[1]);
        strcpy(patron1,argv[2]);
        longueur = strlen(argv[1]);
        chaine = &chain[longueur];
        longueur = strlen(argv[2]);
        patron = &pat[longueur];
     }

    /* trouver la position ou commence l'expression dans la chaine */

    if(*patron == ']')    /*  classe de caracteres */
        chaine = trouve_pos(chaine,patron);
    else
    {
        while(*chaine)
        {
            if(*patron != *chaine)
                   chaine--;
            else
                   break;
         }
     }


    if(*chaine == '\0')
     {
           match = 0;
     }
     
     position = verifie(chaine,patron);

     if(match)
     {
        if(retourne_extention)
              printf("%s",position);
        else
        {
             *position = '\0';
             printf("%s",chaine1);
        }
     }
     else
             printf("%s",chaine1);



}

char *trouve_pos(chaine, patron)
char *chaine, *patron;
{

          char *debut, i;
          int nombre;

          nombre=0;
          while (*(patron-nombre) != '[')
          {
                nombre++;
          }
 
          debut = patron-nombre+1;
 
          nombre--;
          while (*chaine)
          {
             for(i = 0; i < nombre ; i++)
             {
                if(*chaine == *(debut+i))
                     return(chaine);
             }
             chaine--;
          }
      
          return((char *) NULL);
}
        
char *verifie(chaine, patron)
char *chaine, *patron;
{

         int classe();

         while(*patron)
         {
             if(*patron == ']')
             {
                patron--;
                match = match && (classe(chaine,patron));
                while(*patron != '[')
                       patron--;

              }
              else
              {
                match = match && ((*patron^*chaine) == 0) ? 1: 0;
              }
              patron--;
              chaine--;
          }
          chaine++;
          return(chaine);
}

int classe(chaine, patron)
char *chaine, *patron;
{
        char *debut;
        int nombre,i;

         nombre=0;
         while (*(patron-nombre) != '[')
            nombre++;
        

        debut = patron-nombre+1;

        for (i = 0; i < nombre ; i++)
        {
            if(*chaine == *(debut+i))
                return(1);
        }

        return(0);
}
