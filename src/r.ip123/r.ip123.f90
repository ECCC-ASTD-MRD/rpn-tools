program conv123
use ISO_C_BINDING
use app
implicit none
include 'rmn/convert_ip123.inc'
external :: ccard
integer, parameter :: MAXARGS=9
character (len=64), dimension(MAXARGS), save :: liste, def, val
character (len=64) :: warn_text
integer :: i, npos, ip1, ip2, ip3, iostat
integer :: k1, k2, k3
real :: v1, v2, v3
logical :: debug

data liste / 's', 'n', 'v', 6 * '-'/
data val   / 'n', 'n', 'n', 6 * ' '/
data def   / 'y', 'y', 'y', 6 * ' '/

npos=1
call ccard(liste,def,val,MAXARGS,npos)
debug = (val(3)(1:1)=='Y')
if(debug) then
  write(app_msg,*)'number of positional parameters =',npos
  call app_log(APP_INFO,app_msg)
  if(npos==3) call app_log(APP_INFO,'(ip) => (value,kind) conversion')
  if(npos==6) call app_log(APP_INFO,'(value,kind) => (ip) conversion')
  do i=1,MAXARGS
    write(0,101)trim(liste(i)),trim(val(i))
101 format("'",A,"' ,","'",A,"'")
  enddo
endif
if(npos /= 3 .and. npos /= 6) call usage(1)
if(npos==3) then
  read(val(4),*,iostat=iostat)ip1
  if(iostat/=0 .or. ip1<0)goto 222
  read(val(5),*,iostat=iostat)ip2
  if(iostat/=0 .or. ip2<0)goto 222
  read(val(6),*,iostat=iostat)ip3
  if(iostat/=0 .or. ip3<0)goto 222
  write(app_msg,*)'converting: ',ip1,ip2,ip3
  call app_log(APP_DEBUG,app_msg)
  iostat = decode_ip(v1,k1,v2,k2,v3,k3,ip1,ip2,ip3)
  if(iand(iostat,CONVERT_ERROR)==CONVERT_ERROR) goto 555
  print 103,'',v1,k1,v2,k2,v3,k3
  if(iostat/=0 .and. val(2)(1:1)=='Y') goto 666
102 format(A,3I12)
endif
if(npos==6) then
  read(val(4),*,iostat=iostat) v1
  if(iostat/=0) goto 333
  read(val(6),*,iostat=iostat) v2
  if(iostat/=0) goto 333
  read(val(8),*,iostat=iostat) v3
  if(iostat/=0) goto 333
  read(val(5),*,iostat=iostat)k1
  if(iostat/=0 .or. k1<0)goto 444
  read(val(7),*,iostat=iostat)k2
  if(iostat/=0 .or. k1<0)goto 444
  read(val(9),*,iostat=iostat)k3
  if(iostat/=0 .or. k1<0)goto 444
  write(app_msg,*)'converting: ',v1,k1,v2,k2,v3,k3
  call app_log(APP_DEBUG,app_msg)
103 format(a,3(G12.6,' ',i3))
  iostat = encode_ip(ip1,ip2,ip3,v1,k1,v2,k2,v3,k3)
  write(app_msg,*)'result: ',ip1,ip2,ip3
  call app_log(APP_DEBUG,app_msg)
  if(iand(iostat,CONVERT_ERROR)==CONVERT_ERROR) goto 555
  print 102,'',ip1,ip2,ip3
  if(iostat/=0 .and. val(2)(1:1)=='Y') goto 666
endif
stop
222 continue
  write(app_msg,*)'Invalid value(s) for ip1/2/3 (',trim(val(4)),' ',trim(val(5)),' ',trim(val(6)),&
    '), expected 3 non negative integers'
  call app_log(APP_ERROR,app_msg)
  call usage(2)
333 continue
  write(app_msg,*)'Invalid value(s) for value1/2/3 (',trim(val(4)),' ',trim(val(6)),' ',trim(val(8)),&
    '), expected 3 valid floating point numbers'
  call app_log(APP_ERROR,app_msg)
  call usage(3)
444 continue
  write(app_msg,*)'Invalid value(s) for kind1/2/3 (',trim(val(5)),' ',trim(val(7)),' ',trim(val(9)),&
     '), expected 3 non negative integers'
  call app_log(APP_ERROR,app_msg)
  call usage(4)
555 continue
  write(app_msg,*)'ip encoding/decoding error, code=', iostat
  call app_log(APP_WARNING,app_msg)
  write(app_msg,*)'         ip1/2/3    =',ip1,ip2,ip3
  call app_log(APP_VERBATIM,app_msg)
  write(app_msg,*)'         value1/2/3 =',v1,v2,v3
  call app_log(APP_VERBATIM,app_msg)
  write(app_msg,*)'         kind1/2/3  =',k1,k2,k3
  call app_log(APP_VERBATIM,app_msg)
  call usage(5)
666 continue
  warn_text="WARNING"
  if(iand(iostat,CONVERT_GUESS)==CONVERT_GUESS) warn_text = trim(warn_text)//" + GUESS"
  if(iand(iostat,CONVERT_GOOD_GUESS)==CONVERT_GOOD_GUESS) warn_text = trim(warn_text)//" + GOOD_GUESS"
  if(iand(iostat,CONVERT_BAD_GUESS)==CONVERT_BAD_GUESS) warn_text = trim(warn_text)//" + BAD_GUESS"
  if(iand(iostat,CONVERT_TERRIBLE_GUESS)==CONVERT_TERRIBLE_GUESS) warn_text = trim(warn_text)//" + TERRIBLE_GUESS"
  write(app_msg,*)'ip encoding/decoding error, code=', iostat
  call app_log(APP_ERROR,app_msg)
  write(app_msg,*)'         ip1/2/3    =',ip1,ip2,ip3
  call app_log(APP_VERBATIM,app_msg)
  write(app_msg,*)'         value1/2/3 =',v1,v2,v3
  call app_log(APP_VERBATIM,app_msg)
  write(app_msg,*)'         kind1/2/3  =',k1,k2,k3
  call app_log(APP_VERBATIM,app_msg)
  call qqexit(6)
end
subroutine usage(code)
integer :: code

write(0,*)'r.ip123: '
write(0,*)'convert ip1 ip2 ip3 to value1 kind1 value2 kind2 value3 kind3'
write(0,*)'r.ip123 [-s] [-n] [-v] -- ip1 ip2 ip3'
write(0,*)'   or'
write(0,*)'r.ip123 ip1 ip2 ip3 [-s] [-n] [-v]'
write(0,*)''
write(0,*)'convert value1 kind1 value2 kind2 value3 kind3 to ip1 ip2 ip3'
write(0,*)'r.ip123 [-s] [-n] [-v] -- value1 kind1 value2 kind2 value3 kind3'
write(0,*)'   or'
write(0,*)'r.ip123 value1 kind1 value2 kind2 value3 kind3 [-s] [-n] [-v] '
write(0,*)''
write(0,*)'-v : print verbose debug information'
write(0,*)'-s : print type as string rather than numeric code'
write(0,*)'-n : warnings are not silent'
call qqexit(code)
end
