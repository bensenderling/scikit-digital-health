! -*- f95 -*-
include "dfft5.f90"



! https://people.sc.fsu.edu/~jburkardt/f_src/fftpack5/fftpack5.html
subroutine sparc_1d(m, x, fs, nfft, fc, amp_thresh, sparc)
    implicit none
    integer(8), intent(in) :: m, nfft
    real(8), intent(in) :: x(m), fs, fc, amp_thresh
    real(8), intent(out) :: sparc
!f2py intent(hide) :: m
    integer(8) :: i, ixf, inxi, inxf
    real(8) :: freq_factor
    real(8) :: Mf(nfft / 2)
    ! fft stuff
    real(8) :: sp_hat(nfft), wsave(nfft + int(log(real(nfft))) + 4)
    real(8) :: wrk(nfft)
    integer(4) :: lensav, lenwrk, ier
    
    ! setup for FFT
    lensav = nfft + int(log(real(nfft))) + 4
    lenwrk = nfft
    call dfft1i(nfft, wsave, lensav, ier)

    ! compute the FFT
    sp_hat = 0._8
    sp_hat(:m) = x
    call dfft1f(nfft, 1_4, sp_hat, nfft, wsave, lensav, wrk, lenwrk, ier)
    ! Normalized magnitude spectrum
    Mf(1) = 2 * sp_hat(1)
    
    do i=2, nfft, 2
        Mf(i/2+1) = sqrt(sp_hat(i)**2 + sp_hat(i+1)**2)
    end do
    Mf = Mf / maxval(Mf)
    
    ! frequency cutoff index.  This will essentially function as a low-pass filter
    ! to remove high frequency noise from affecting the next step
    ixf = ceiling(fc / (0.5 * fs) * (nfft / 2))
    
    ! choose the amplitude threshold based cutoff frequency
    ! Index of the end points on the magnitude spectrum that are greater than
    ! or equal to the amplitude threshold
    inxi = 1_8
    inxf = ixf
    do while ((Mf(inxi) < amp_thresh) .AND. (inxi < (nfft/2)))
        inxi = inxi + 1
    end do
    do while ((Mf(inxf) < amp_thresh) .AND. (inxf > 1))
        inxf = inxf - 1
    end do
    
    ! compute the arc length
    freq_factor = 1.0_8 / (inxf - inxi)**2

    do i=inxi+1, inxf
        sparc = sparc - sqrt(freq_factor + (Mf(i) - Mf(i-1))**2)
    end do
end subroutine


subroutine SPARC(m, n, p, x, fs, padlevel, cut, amp_thresh, sal)
    implicit none
    integer(8), intent(in) :: m, n, p, padlevel
    real(8), intent(in) :: x(m, n, p), fs, cut, amp_thresh
    real(8), intent(out) :: sal(n, p)
!f2py intent(hide) :: m, n, p
    integer(8) :: j, k, nfft
    
    nfft = 2**(ceiling(log(real(m))/log(2.0)) + padlevel)
    
    do k=1, p
        do j=1, n
            call sparc_1d(m, x(:, j, k), fs, nfft, cut, amp_thresh, sal(j, k))
        end do
    end do
end subroutine