	program makebidon
	implicit none
	character *12  nomprg
	character *60  nomabs
	integer long_111, c_to_f_sw, lng, ier
	external c_to_f_sw
	nomprg = 'null'

	nomprg='            '
c        0        1         2         3         4         5         6 
c	 123456789012345678901234567890123456789012345678901234567890
	nomabs=
     %  '                                                            '
	call getarg(1,nomprg)
	call getarg(0,nomabs)
	lng = long_111(nomprg)
	ier = c_to_f_sw(nomprg,lng)
	
	stop
	end


      INTEGER FUNCTION LONG_111(NOM)
      implicit none
      CHARACTER * (*) NOM
*
      INTEGER LNG,I
*
      LNG = LEN(NOM)
      DO 10 I = LNG,1,-1
         IF (NOM(I:I) .EQ. ' ') THEN
            LNG = LNG - 1
         ELSE
            GOTO 20
         ENDIF
 10   CONTINUE
 20   CONTINUE
      LONG_111 = LNG
      RETURN
      END

