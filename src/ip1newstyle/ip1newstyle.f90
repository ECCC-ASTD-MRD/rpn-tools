
      program ip1newstyle
      use app

!author vivian lee Oct.4,2002
!
!object
!  To convert IP1 oldstyle to IP1 newstyle given one or more FSTD files
!  into one FSTD destination file
!
!  ip1newstyle -s file1 file2 file3 file4 [...file34] -d ip1new_file_98
!  ip1newstyle -s file1 file2 file3 file4 [...file34]

      implicit none
      integer work(10)
      real champ,lev
      pointer (ptfield,champ(1))

      integer key, dte, det, ipas, ni, nj, nk, nbits, dty
      integer ip1a, ip1, ip2, ip3, ig1, ig2, ig3, ig4, swa, lng, dlf
      integer ubc, ex1, ex2, ex3

      character *1 blk_S
      character *2 grd, tva
      character *4 var
      character *12 etiket

      integer iun(35), ier, errcode, abort, alloue
      integer ni1,nj1,nk1,kind,nsrcfile,i,j,k

      character *8 cles(36)
      character *256 def(36), val(36)

      integer  fnom,fstouv,fstecr,fstfrm,fstprm,fstluk,fstlnk,fstsui,fstinf
      external fnom,fstouv,fstecr,fstfrm,fstprm,fstluk,fstlnk,fstsui,fstinf

      data alloue /20000/
      data cles /'D.',35*'S.'/
      data def /'ip1new_file_98', 35*' '/
      data val /'ip1new_file_98', 35*' '/

      ier = -111
      nsrcfile=0
      alloue = 20000
      call ccard(cles,def,val,35,ier)
      call hpalloc(ptfield,alloue,errcode,abort)

      do i=1,35
         if (val(i+1) .ne. ' ') nsrcfile= nsrcfile+1
      enddo

      ier = 0

!     Open all the input files to be converted and link them together


      if (nsrcfile .gt. 0) then
          do i = 1,nsrcfile
             iun(i)=i+24
             write(app_msg,*) 'iun(',i,')=',iun(i),'val(',i+1,')=',val(i+1)
             call app_log(APP_DEBUG,app_msg)
             if (fnom(iun(i),val(i+1),'STD+RND',0) .ge. 0) then
                write(app_msg,*) 'Opening ',iun(i)
                call app_log(APP_INFO,app_msg)
                ier = fstouv(iun(i),'STD+RND')
             else
                write(app_msg,*) 'Problems opening source file',val(i+1)
                call app_log(APP_ERROR,app_msg)
             stop
             endif
          enddo
          if (nsrcfile.gt.1) ier = fstlnk(iun,nsrcfile)
      else
         call app_log(APP_ERROR,'No source files found')
          stop
      endif

!     Open the destination file

      ier = fnom(20,val(1),'STD+RND',0)
      ier = fstouv(20,'STD+RND')
      if (ier .lt. 0) then
         write(app_msg,*) 'Problems opening destination file',val(i)
         call app_log(APP_ERROR,app_msg)
         stop
      endif


!     Set the encoding of IP1 to be in the newstyle by default

      call convip_plus (ip1, lev, kind,0, blk_S, .false.)


!     Look for the first record in the file

      key = fstinf(iun(1),ni1,nj1,nk1,-1,' ',-1,-1,-1,' ',' ')
      do while (key .ge. 0)
         ier = fstprm (key,dte,det,ipas,ni,nj,nk,nbits,dty,ip1a,ip2,ip3,&
            tva,var,etiket,grd,ig1,ig2,ig3,ig4,swa,lng,dlf,ubc,ex1,ex2,ex3)

!     Make sure there is enough memory for field to be read in

         if (ni*nj*nk .gt. alloue) then
             alloue = ni*nj*nk
             call hpdeallc(ptfield,errcode,abort)
             call hpalloc(ptfield,alloue,errcode,abort)
             write(app_msg,*) 'Memory allocation is increased: ni,nj,nk=',ni,nj,nk
             call app_log(APP_INFO,app_msg)
         endif
!     Read the field
         ier = fstluk(champ,key,ni1,nj1,nk1)

!     Avoid doing any IP1 conversion for records '^^','>>' and 'HY'

         if (var.eq.'^^'.or.var.eq.'>>'.or.var.eq.'HY') then
             ip1 = ip1a
         else 
             call convip_plus (ip1a, lev, kind,-1, blk_S, .false.)
             call convip_plus (ip1,  lev, kind,+1, blk_S, .false.)
         endif
!     Write the field
         ier = fstecr(champ, WORK, -nbits, 20, dte, det, ipas, &
                  ni, nj, nk, ip1, ip2, ip3, tva, var, etiket, &
                  grd, ig1, ig2, ig3, ig4, dty, .false.)
!     Look for next record
         key = fstsui(iun(1),ni1,nj1,nk1)
      end do

!     Close all files
      ier = fstfrm(iun(1))
      ier = fstfrm(20)
      call hpdeallc(ptfield,errcode,abort)
      stop
      end
      
      character *128 function product_id_tag()
      product_id_tag='$Id$'
      return
      end
