
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
   r.ucanrm - verifier si l'usager appelant est le 
   proprietaire d'un fichier et regarder si
   le proprietaire a la permission d'ecriture pour
   ce fichier
   r.ucanrm retourne 1 si il y a conflit usager
                        ou si le fichiers a
                        une permission d'ecriture
                        pour le proprio (ce qui implique
                        que l'usager a modifie le fichier
                        et qu'on doit conserver ses modifications
                        (s'applique au contexte etagere et a make clean)).
                        
            retourne 0  si le fichier est read pour 
                        le proprio. On peut donc l'effacer

                        Note: on retourne 0 pour vrai et 1 pour faux
                        selon le mode d'utilisation du if en Bourne shell

   Auteur: James Caveen 

   appel:   r.ucanrm fichier
   usage dans un script Bourne:   if(r.ucanrm fichier) 
                                  then 
                                     rm fichier
                                  fi
*/
                       
int main(argc, argv)
int argc;
char *argv[];
{
    int ier;
    struct stat buf;

    ier = stat(argv[1],&buf);

    /*
       Verifier l'identite de l'usager 
    */

    if((int) buf.st_uid != (int) getuid())
       exit(1);

    /*
       Verifier si le fichier est en mode 
       ecriture pour le proprietaire
    */

    if(buf.st_mode & S_IWUSR)
       exit(1);

    exit(0);
}
