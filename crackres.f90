program crackres
  implicit none

! Author: Simon Pellerin (ARMA), Dec. 2001
! 
! OBJECT: 
!   Extract Synoptic DateTime and resume ID out of resume record 
!   of a BURP observation file.
!
! CCARD Input :
!         -ibrp : BURP FILE
!
! Standard output on a single line: <YYYYMMDDHH> <RESUME ID>
!
  integer :: ier,mrfopc,ibrp1,fnom,inblks,numblks,mrfopn,inrecs
  integer :: ilong, mrfmxl, idate,itime,irunn,ihandl,mrfloc,mrfget
  integer, allocatable, dimension(:) :: ibuf
  integer :: IFLGS,idburp,ilat,ilon,idx,idy,ialt,idelay,irs,inblk,isup,insup,ixaux,inxaux
  character (len=9) :: clstnid
  integer :: mrfcls, fclos, mrbhdr, newdate,istampobs,ibrpstamp,ibrpdate,inewhh
  integer :: ibrphh,iposit, infil
  real*8 :: delhh
  character (len=5) :: cle
  character (len=128) :: cldef,clvalu

  iposit = 0
  infil = 1
  cle = 'ibrp.'
  cldef = 'ISCRAP'
  clvalu = 'ISCRAP'

  call ccard(cle,cldef,clvalu,infil,iposit)

  if(clvalu /= 'ISCRAP') then
     ibrp1 = 0
     ier = fnom(IBRP1,clvalu,'RND+OLD',0)

     if ( ier == 0 ) then
        ier = mrfopc('MSGLVL','FATAL')
        idate = -9999
        itime = -9999
        irunn = -9999
        
        inblks= -1
        inblks = numblks(ibrp1)
        
        if ( inblks > 0 ) then
           inrecs = mrfopn(ibrp1,'READ')
           ilong  = mrfmxl(ibrp1)
           allocate(ibuf(ilong+20))
           ibuf(1)= ilong + 20
           ! Looking for a resume record id
           ihandl = mrfloc(ibrp1,0,'>>*******',-1,-1,-1,-1,-1,-1,0)
           if ( ihandl < 0 ) then
              ! if not found get the first station id
              ihandl = mrfloc(ibrp1,0,'*********',-1,-1,-1,-1,-1,-1,0)
           endif
           if ( ihandl < 0 ) then
              ! no valid handle : return noting
              WRITE(*,*) 'AUCUN ENREGISTREMENT VALIDE DANS LE FICHIER BURP'
           else
              ier = mrfget(ihandl,ibuf)
              ier = mrbhdr(ibuf,ITIME,IFLGS,CLSTNID,IDBURP,ILAT,ILON,IDX,IDY,&
                   IALT,IDELAY,IDATE,IRS,IRUNN,INBLK,ISUP,INSUP,IXAUX,INXAUX)
              
              ier = newdate(istampobs,idate,itime,3)
              delhh = 3.0
              call incdatr (ibrpstamp, istampobs, delhh)
              
              ier = newdate(ibrpstamp,ibrpdate,inewhh,-3)
              ibrphh = itime/100
              ! Compose Synoptic time in case that we fell on an observation
              ! record
              if (ibrphh .ge. 21 .or. ibrphh .lt. 3) then
                 ibrphh = 0
              elseif(ibrphh .ge. 3 .and. ibrphh .lt. 9) then
                 ibrphh = 6
              elseif(ibrphh .ge. 9 .and. ibrphh .lt. 15) then
                 ibrphh = 12
              else
                 ibrphh = 18
              endif
              ier = newdate(ibrpstamp,ibrpdate,ibrphh*1000000,3)
              ibrpdate = ibrpdate*100 + ibrphh
              ! print them out
              WRITE(*, '(i10,1x,a9)') ibrpdate,clstnid
           endif
           deallocate(ibuf)
        endif
        ier = mrfcls(ibrp1)
        ier = fclos(ibrp1)
     endif
  endif

end program crackres
