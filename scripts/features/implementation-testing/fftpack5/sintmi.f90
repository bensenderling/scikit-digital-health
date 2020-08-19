! -*- f90 -*-
include "xerfft.f90"
include "rfftmi.f90"
include "xerfft.f90"
subroutine sintmi ( n, wsave, lensav, ier )

!*****************************************************************************80
!
!! SINTMI: initialization for SINTMB and SINTMF.
!
!  Discussion:
!
!    SINTMI initializes array WSAVE for use in its companion routines 
!    SINTMF and SINTMB.  The prime factorization of N together with a 
!    tabulation of the trigonometric functions are computed and stored 
!    in array WSAVE.  Separate WSAVE arrays are required for different 
!    values of N.
!
!  License:
!
!    Licensed under the GNU General Public License (GPL).
!    Copyright (C) 1995-2004, Scientific Computing Division,
!    University Corporation for Atmospheric Research
!
!  Modified:
!
!    02 April 2005
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
!    Input, integer ( kind = 4 ) N, the length of each sequence to be 
!    transformed.  The transform is most efficient when N is a product of 
!    small primes.
!
!    Input, integer ( kind = 4 ) LENSAV, the dimension of the WSAVE array.  
!    LENSAV must be at least N/2 + N + INT(LOG(REAL(N))) + 4.
!
!    Output, real ( kind = 4 ) WSAVE(LENSAV), containing the prime factors
!    of N and also containing certain trigonometric values which will be used
!    in routines SINTMB or SINTMF.
!
!    Output, integer ( kind = 4 ) IER, error flag.
!    0, successful exit;
!    2, input parameter LENSAV not big enough;
!    20, input error returned by lower level routine.
!
  implicit none

  integer ( kind = 4 ) lensav

  real ( kind = 4 ) dt
  integer ( kind = 4 ) ier
  integer ( kind = 4 ) ier1
  integer ( kind = 4 ) k
  integer ( kind = 4 ) lnsv
  integer ( kind = 4 ) n
  integer ( kind = 4 ) np1
  integer ( kind = 4 ) ns2
  real ( kind = 4 ) pi
  real ( kind = 4 ) wsave(lensav)

  ier = 0

  if ( lensav < n / 2 + n + int ( log ( real ( n, kind = 4 ) ) ) + 4 ) then
    ier = 2
    call xerfft ( 'sintmi', 3 )
    return
  end if

  pi = 4.0E+00 * atan ( 1.0E+00 )

  if ( n <= 1 ) then
    return
  end if

  ns2 = n / 2
  np1 = n + 1
  dt = pi / real ( np1, kind = 4 )

  do k = 1, ns2
    wsave(k) = 2.0E+00 * sin ( real ( k, kind = 4 ) * dt )
  end do

  lnsv = np1 + int ( log ( real ( np1, kind = 4 ) ) ) + 4

  call rfftmi ( np1, wsave(ns2+1), lnsv, ier1 )

  if ( ier1 /= 0 ) then
    ier = 20
    call xerfft ( 'sintmi', -5 )
    return
  end if

  return
end
function xercon ( inc, jump, n, lot )

!*****************************************************************************80
!
!! XERCON checks INC, JUMP, N and LOT for consistency.
!
!  Discussion:
!
!    Positive integers INC, JUMP, N and LOT are "consistent" if,
!    for any values I1 and I2 < N, and J1 and J2 < LOT,
!
!      I1 * INC + J1 * JUMP = I2 * INC + J2 * JUMP
!
!    can only occur if I1 = I2 and J1 = J2.
!
!    For multiple FFT's to execute correctly, INC, JUMP, N and LOT must
!    be consistent, or else at least one array element will be
!    transformed more than once.
!
!  License:
!
!    Licensed under the GNU General Public License (GPL).
!    Copyright (C) 1995-2004, Scientific Computing Division,
!    University Corporation for Atmospheric Research
!
!  Modified:
!
!    25 March 2005
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
!    Input, integer ( kind = 4 ) INC, JUMP, N, LOT, the parameters to check.
!
!    Output, logical XERCON, is TRUE if the parameters are consistent.
!
  implicit none

  integer ( kind = 4 ) i
  integer ( kind = 4 ) inc
  integer ( kind = 4 ) j
  integer ( kind = 4 ) jnew
  integer ( kind = 4 ) jump
  integer ( kind = 4 ) lcm
  integer ( kind = 4 ) lot
  integer ( kind = 4 ) n
  logical              xercon

  i = inc
  j = jump

  do while ( j /= 0 )
    jnew = mod ( i, j )
    i = j
    j = jnew
  end do
!
!  LCM = least common multiple of INC and JUMP.
!
  lcm = ( inc * jump ) / i

  if ( lcm <= ( n - 1 ) * inc .and. lcm <= ( lot - 1 ) * jump ) then
    xercon = .false.
  else
    xercon = .true.
  end if

  return
end
