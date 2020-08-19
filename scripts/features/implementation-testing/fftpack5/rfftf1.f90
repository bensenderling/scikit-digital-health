! -*- f90 -*-
include "r1fgkf.f90"
include "r1fgkf.f90"
include "r1f5kf.f90"
include "r1f5kf.f90"
include "r1f3kf.f90"
include "r1f3kf.f90"
include "r1f2kf.f90"
include "r1f2kf.f90"
include "r1f4kf.f90"
include "r1f4kf.f90"
subroutine rfftf1 ( n, in, c, ch, wa, fac )

!*****************************************************************************80
!
!! RFFTF1 is an FFTPACK5 auxiliary routine.
!
!  License:
!
!    Licensed under the GNU General Public License (GPL).
!    Copyright (C) 1995-2004, Scientific Computing Division,
!    University Corporation for Atmospheric Research
!
!  Modified:
!
!    27 March 2009
!
!  Author:
!
!    Paul Swarztrauber
!    Richard Valent
!
!  Reference:
!
!    Paul Swarztrauber,
!    Vectorizing the Fast Fourier Transforms,
!    in Parallel Computations,
!    edited by G. Rodrigue,
!    Academic Press, 1982.
!
!    Paul Swarztrauber,
!    Fast Fourier Transform Algorithms for Vector Computers,
!    Parallel Computing, pages 45-63, 1984.
!
!  Parameters:
!
  implicit none

  integer ( kind = 4 ) in
  integer ( kind = 4 ) n

  real ( kind = 4 ) c(in,*)
  real ( kind = 4 ) ch(*)
  real ( kind = 4 ) fac(15)
  integer ( kind = 4 ) idl1
  integer ( kind = 4 ) ido
  integer ( kind = 4 ) ip
  integer ( kind = 4 ) iw
  integer ( kind = 4 ) ix2
  integer ( kind = 4 ) ix3
  integer ( kind = 4 ) ix4
  integer ( kind = 4 ) j
  integer ( kind = 4 ) k1
  integer ( kind = 4 ) kh
  integer ( kind = 4 ) l1
  integer ( kind = 4 ) l2
  integer ( kind = 4 ) modn
  integer ( kind = 4 ) na
  integer ( kind = 4 ) nf
  integer ( kind = 4 ) nl
  real ( kind = 4 ) sn
  real ( kind = 4 ) tsn
  real ( kind = 4 ) tsnm
  real ( kind = 4 ) wa(n)

  nf = int ( fac(2) )
  na = 1
  l2 = n
  iw = n

  do k1 = 1, nf

    kh = nf - k1
    ip = int ( fac(kh+3) )
    l1 = l2 / ip
    ido = n / l2
    idl1 = ido * l1
    iw = iw - ( ip - 1 ) * ido
    na = 1 - na

    if ( ip == 4 ) then

      ix2 = iw + ido
      ix3 = ix2 + ido

      if ( na == 0 ) then
        call r1f4kf ( ido, l1, c, in, ch, 1, wa(iw), wa(ix2), wa(ix3) )
      else
        call r1f4kf ( ido, l1, ch, 1, c, in, wa(iw), wa(ix2), wa(ix3) )
      end if

    else if ( ip == 2 ) then

      if ( na == 0 ) then
        call r1f2kf ( ido, l1, c, in, ch, 1, wa(iw) )
      else
        call r1f2kf ( ido, l1, ch, 1, c, in, wa(iw) )
      end if

    else if ( ip == 3 ) then

      ix2 = iw + ido

      if ( na == 0 ) then
        call r1f3kf ( ido, l1, c, in, ch, 1, wa(iw), wa(ix2) )
      else
        call r1f3kf ( ido, l1, ch, 1, c, in, wa(iw), wa(ix2) )
      end if

    else if ( ip == 5 ) then

      ix2 = iw + ido
      ix3 = ix2 + ido
      ix4 = ix3 + ido

      if ( na == 0 ) then
        call r1f5kf ( ido, l1, c, in, ch, 1, wa(iw), wa(ix2), wa(ix3), wa(ix4) )
      else
        call r1f5kf ( ido, l1, ch, 1, c, in, wa(iw), wa(ix2), wa(ix3), wa(ix4) )
      end if

    else

      if ( ido == 1 ) then
        na = 1 - na
      end if

      if ( na == 0 ) then
        call r1fgkf ( ido, ip, l1, idl1, c, c, c, in, ch, ch, 1, wa(iw) )
        na = 1
      else
        call r1fgkf ( ido, ip, l1, idl1, ch, ch, ch, 1, c, c, in, wa(iw) )
        na = 0
      end if

    end if

    l2 = l1

  end do

  sn = 1.0E+00 / real ( n, kind = 4 )
  tsn = 2.0E+00 / real ( n, kind = 4 )
  tsnm = -tsn
  modn = mod ( n, 2 )
  nl = n - 2

  if ( modn /= 0 ) then
    nl = n - 1
  end if

  if ( na == 0 ) then

    c(1,1) = sn * ch(1)
    do j = 2, nl, 2
      c(1,j) = tsn * ch(j)
      c(1,j+1) = tsnm * ch(j+1)
    end do

    if ( modn == 0 ) then
      c(1,n) = sn * ch(n)
    end if

  else

    c(1,1) = sn * c(1,1)

    do j = 2, nl, 2
      c(1,j) = tsn * c(1,j)
      c(1,j+1) = tsnm * c(1,j+1)
    end do

    if ( modn == 0 ) then
      c(1,n) = sn * c(1,n)
    end if

  end if

  return
end
