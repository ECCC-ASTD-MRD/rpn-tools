      program filetyp
      implicit none
      character *8 cle
      character *4096 def(1),val(1)
      character *60 msgs(-1:34)
      integer wkoffit,ipos,ier

      data cle /'-.'/
      data def /' '/
      data val /' '/

      data msgs(-1) /'unknown'/
      data msgs(0)  /'error: undefined value, 0'/
      data msgs(1)  /'RPN standard random 89'/
      data msgs(2)  /'RPN standard sequential 89'/
      data msgs(3)  /'RPN standard sequential fortran 89'/
      data msgs(4)  /'CCRN'/
      data msgs(5)  /'CCRN-RPN'/
      data msgs(6)  /'BURP'/
      data msgs(7)  /'GRIB'/
      data msgs(8)  /'BUFR'/
      data msgs(9)  /'BLOK'/
      data msgs(10) /'FORTRAN sequential unformatted'/
      data msgs(11) /'COMPRESS'/
      data msgs(12) /'GIF89'/
      data msgs(13) /'GIF87'/
      data msgs(14) /'IRIS'/
      data msgs(15) /'JPEG'/
      data msgs(16) /'KMW'/
      data msgs(17) /'PBM'/
      data msgs(18) /'PCL'/   
      data msgs(19) /'PCX'/   
      data msgs(20) /'PDSVICAR'/
      data msgs(21) /'PM'/      
      data msgs(22) /'PPM'/     
      data msgs(23) /'POSTCRIPT'/
      data msgs(24) /'KMW_'/
      data msgs(25) /'RRBX'/    
      data msgs(26) /'SUNRAS'/  
      data msgs(27) /'TIFF'/    
      data msgs(28) /'UTAHRLE'/ 
      data msgs(29) /'XBM'/     
      data msgs(30) /'XWD'/     
      data msgs(31) /'ASCII'/     
      data msgs(32) /'BMP'/     
      data msgs(33) /'RPN standard random 98'/
      data msgs(34) /'RPN standard sequential 98'/


      ipos = 0
      call ccard (cle,def,val,1,ipos)

      ier = wkoffit(val(1))
      if (ier .gt. -1) then
        write(6,77) 'File type is ',msgs(ier)
      else if (ier .eq. -1) then
         call system(
     %   'file -m /usr/local/env/armnlib/data/magic.extra '//val(1))
         call system('file '//val(1))
      else if (ier .eq. -2) then
         write(6,99)
      else
        write(6,88)
      endif

 77   format(/,a,a60,/)
 88   format(/,'File does not exist or can not open',/)
 99   format(/,'File empty',/)

      call qqexit(ier)
      stop
      end
      
      character *128 function product_id_tag()
      implicit none
      product_id_tag='$Id$'
      return
      end
