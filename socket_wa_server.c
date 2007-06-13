/* RMNLIB - Library of useful routines for C and FORTRAN programming
 * Copyright (C) 1975-2000  Division de Recherche en Prevision Numerique
 *                          Environnement Canada
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// int connect_to_channel_by_name(char *name);

main(int argc, char **argv)
{
  int ipaddress,port,nc;
  char buf[1024], tempout[256], temperr[256];
  char ftype[128] = {"RND,"};
  char *ctemp;
  int *wa_buf;
  int max_n = 1024;
  int fdesc;
  int iun, ier, nlu, necrit, c_iun, not_over=1;
  int access_mode=F_OK, new_checksum;
  int *s_ID, *addr, *nw, *RW_mode, *checksum;
  		/* RW_mode = 1  		read request */
      /* RW_mode = 2 			write request */
      /* RW_mode = 3 			close request */
  int sock_comm_ID=0xBABE;
  int demande[5];

  s_ID = &(demande[0]);
  addr = &(demande[1]);
  nw = &(demande[2]);
  RW_mode = &(demande[3]);
  checksum = &(demande[4]);
  *s_ID = sock_comm_ID;
  
  if (strcasecmp(argv[2],"R/O") == 0) {
    access_mode = R_OK;
    if (access(argv[1],access_mode) == -1)
      printf("NR/O");
    else
      printf("ACK");
  }
  else if ((strcasecmp(argv[2],"R/W") == 0) || (strcasecmp(argv[2],"APPEND") == 0)) {
    access_mode = W_OK;
    if (access(argv[1],F_OK) == -1)     /* file does not exist, create */
      printf("ACK");
    else {
      if (access(argv[1],access_mode) == -1)
        printf("NR/W");
      else
        printf("ACK");
    }
  }  
  fflush(stdout);
  close(0);
  close(1);
  close(2);

  if(fork()) exit(0);

  snprintf(tempout,256,"%s_%i_%i","/tmp/WA_Socket_stdout",getuid(),getpid());
  snprintf(temperr,256,"%s_%i_%i","/tmp/WA_Socket_stderr",getuid(),getpid());
  freopen("/dev/null","a",stdin);
  freopen(tempout,"a",stdout);
  freopen(temperr,"a",stderr);
  setpgrp();
//  sleep(10);
//  exit(0);
  printf("connecting\n");
  fflush(stdout);
  printf("arguments a l'appel %s %s %s\n",argv[1],argv[2],argv[3]);
  fflush(stdout);
  iun = 0;
  ctemp = strcat(ftype,argv[2]);
  ier = c_fnom(&iun,argv[1],ftype,0);
//  ier = c_fnom(&iun,argv[1],"RND",0);
  if (ier < 0) {
    fprintf(stderr,"c_fnom error: iun=%d err=%d\n",iun,ier);
    exit(1);
  }
  printf("fnom file=%s type=%s ier=%d iun=%d\n",argv[1],ftype,ier,iun);
  c_waopen(iun);
  fdesc=connect_to_hostport(1+argv[3]);
  if(fdesc <= 0) {
    fprintf(stderr,"connect_to_hostport: unable to connect\n");
    exit(2);
  }
  printf("fdesc=%d\n",fdesc);
  fflush(stdout);
  wa_buf = calloc(1024,sizeof(int));
//  nc=read(fdesc,buf,1023);
	while (not_over) {
    nc=read_stream(fdesc,demande,5*sizeof(int));
    check_swap_records(demande,nc/sizeof(int),sizeof(int));
    printf("read (demande) %d bytes from descriptor %d \n",nc,fdesc);
    fflush(stdout);
  //  sscanf(buf,"%d %d %d",&c_iun,&addr,&nw);
    new_checksum = *s_ID ^ *addr ^ *nw ^ *RW_mode;
    printf("demande de RW_mode=%d addr=%d nw=%d\n",*RW_mode,*addr,*nw);
    fflush(stdout);  
    if (new_checksum != *checksum) {
      fprintf(stdout,"Debug+ checksum mismatch\n");
      fflush(stdout);
      fprintf(stderr,"wa_socket_server error: invalid checksum\n");
      fflush(stderr);
      close(fdesc);
      }
  	if (*RW_mode == 3) {   /* close request */
      printf("Debug+ socket_wa_server requete de FERMETURE de session\n");
      unlink(tempout);
      unlink(temperr);
      not_over = 0;
      break;
      }
    printf("Debug+ socket_wa_server *nw=%d max_n=%d\n",*nw,max_n);
    fflush(stdout);
    if (*nw > max_n) {
      free(wa_buf);
      wa_buf = calloc(*nw,sizeof(int));
      max_n = *nw;
    }
    if (*RW_mode == 1) {  /* read request */
      printf("Debug+ socket_wa_server requete de LECTURE\n");
      fflush(stdout);
      nlu = c_waread2(iun,wa_buf,*addr,*nw);
      if (nlu != *nw) {
        fprintf(stderr,"socket_wa_server read error, request nw=%d, read=%d\n",*nw,nlu);
        close(fdesc);
        exit(3);
      }
      check_swap_records(wa_buf,*nw,sizeof(int));
      nc=write_stream(fdesc,wa_buf,*nw * sizeof(int));
      printf("wrote %d bytes to descriptor %d at address %s\n",*nw * sizeof(int),fdesc,1+argv[3]);
    }
    else if (*RW_mode == 2) {   /* write request */
      printf("Debug+ socket_wa_server requete d'ECRITURE\n");
      fflush(stdout);
    	nc=read_stream(fdesc,wa_buf,*nw * sizeof(int));
      printf("Debug+ socket_wa_server write request nc=%d bytes \n",nc);
      if (nc != (*nw * sizeof(int))) {
        fprintf(stderr,"socket_wa_server write request: read_stream error, expecting %d bytes , got %d\n",
        				*nw,nc);
        close(fdesc);
        exit(3);
      }     
      check_swap_records(wa_buf,*nw,sizeof(int));
      necrit = c_wawrit2(iun,wa_buf,*addr,*nw);
      printf("Debug+ socket_wa_server necrit=%d words\n",necrit);
      if (necrit != *nw) {
        fprintf(stderr,"socket_wa_server write error, request nw=%d, written=%d\n",nw,necrit);
        close(fdesc);
        exit(4);
      }
    }
    else if (*RW_mode == 4) {  /* wasize request */
      printf("Debug+ socket_wa_server requete WASIZE\n");
      fflush(stdout);
      *nw = c_wasize(iun);
      printf("Debug+ socket_wa_server wasize=%d\n",*nw);
      fflush(stdout);
      *addr = 0;
      *checksum = *s_ID ^ *addr ^ *RW_mode;      /* nw can't be part of checksum */
      check_swap_records(demande,5,sizeof(int));
      nc=write_stream(fdesc,demande,5 * sizeof(int));
      if (nc != 0) {
        fprintf(stderr,"socket_wa_server error: write_stream=%d\n",nc);
        fflush(stderr);
        }
      else {
        printf("wrote to descriptor %d OK\n",fdesc);
        fflush(stdout);
        }
    }
    else {
      fprintf(stderr,"socket_wa_server error: invalid request=%d\n",*RW_mode);
      close(fdesc);
      exit(5);
    }
  }
  printf("Debug+ socket_wa_server fin de la boucle, ferme et termine\n");
  c_waclos(iun);
  close(fdesc);
}
