#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ncarmax  255
#define ncarplus 256
int main(int argc, char *argv[])

{
     FILE *entree;

     int ligne, nlignes=1;

     char buffer[ncarplus];
     char reference[ncarplus];

/*   ouverture du fichier d'entree  */
     if( (entree = fopen (argv[1], "r")) == (FILE *) NULL)
     {
        fprintf(stderr, "On ne peut lire sur %s \n",argv[1]);
        exit(1);
     }

/*   trouver la ligne de reference
     premiere ligne contenant des caracteres
*/

     while((ligne = fgets(reference,ncarmax,entree) != NULL))
     {

          if((*reference == '\n') || (*reference == '\0'))
          {
              ;
          }
          else
          {
               break;
          }
     }


/*   boucle sur toutes les lignes du fichier d'entree 
     pour trouver la prochaine ligne correspondant a
     la ligne de reference
 */
     while((ligne = fgets(buffer,ncarmax,entree) != NULL))
     {
         if (strcmp(reference,buffer))
         {
            nlignes++;
         }
         else
         {
            break;
         } 
     }
   
     nlignes = (nlignes > 66) ? 66 : nlignes;
     nlignes = (nlignes < 60) ? 60 : nlignes;

     printf("%d",nlignes);


/*   fermeture des fichiers 
*/

     fclose(entree);

     exit(0);

}
