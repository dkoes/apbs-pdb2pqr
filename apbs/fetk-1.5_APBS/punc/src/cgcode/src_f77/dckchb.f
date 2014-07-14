      SUBROUTINE DCKCHB(IPAR,RPAR,PEGMIN,PEGMAX,CONDCA)
C***BEGIN PROLOGUE  DCKCHB
C***REFER TO  DPPCG,DPCGCA
C***ROUTINES CALLED  DDPCHB,D1MACH
C***REVISION DATE  870401   (YYMMDD)
C***END PROLOGUE  DCKCHB
C
C     THIS SUBR OUTPUTS THE POLYNOMIAL INPUT PARAMETERS NDEG, AA, BB.
C     IT THEN CALLS DDPCHB (WITH IOUNIT=0) TO GET THE CORRESPONDING
C     MIN AND MAX EGVALS OF C(A)A.  FINALLY, THE INITIAL CONDCA IS
C     COMPUTED FOR USE IN THE STOPPING CRITERION.  
C
C     *** DECLARATIONS ***
      IMPLICIT  DOUBLE PRECISION(A-H,O-Z)
      DIMENSION IPAR(*),RPAR(*)
C
C***FIRST EXECUTABLE STATEMENT  DCKCHB
 1    CONTINUE
C
C     *** OUTPUT THE PARAMETERS ***
      IOUNIT = IPAR(5)
      NDEG = IPAR(34)
      NCHB = NDEG+1
      AA = RPAR(32)
      BB = RPAR(33)
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,5) NDEG, AA, BB
 5    FORMAT(4X, 'NDEG   = ', I5, /, 4X, 'AA     = ', D12.5, /,
     2       4X, 'BB     = ', D12.5)
C
C     *** CHECK FOR IMPROPER NDEG ***
      IF (NDEG .LT. 0) WRITE(IOUNIT,10) 
 10   FORMAT(/, ' WARNING IN DCKCHB: NDEG .LT. 0; NO PRECONDG DONE', /)
      IF ((MOD(NCHB,2) .EQ. 0) .AND. (NCHB .GE. 0)) WRITE(IOUNIT,11) 
 11   FORMAT(/, ' WARNING IN DCKCHB: NDEG IS ODD; NO ADAPTIVE DONE', /)
C
C     *** CHECK FOR INVALID AA, BB ***
      IF ((AA .LE. 0) .OR. (AA .GT. BB)) THEN
         WRITE(IOUNIT,15) 
 15      FORMAT(/, ' ERROR IN DCKCHB: (AA .LE. 0) OR (AA .GT. BB)')
         STOP
      END IF
C
C     *** DETERMINE PEGMIN, PEGMAX, CONDCA ***
      IF ((NDEG .LT. 0) .OR. (MOD(NCHB,2) .EQ. 0)) THEN
         PEGMIN = D1MACH(2)
         PEGMAX = D1MACH(1)
      ELSE
         PEGMIN = 1.0D0
         PEGMAX = 1.0D0
      END IF
      CONDCA = 1.0D0
      ERRTOL  = RPAR(1)
      CALL DDPCHB(0,AA,BB,NCHB,PEGMIN,PEGMAX,CONDCA,ERRTOL,IADAPT)
      IF (IADAPT .GT. 0) WRITE(IOUNIT,20)
 20   FORMAT(/,' WARNING IN DCKCHB: INITIAL AA,BB CHANGED', /)
C
C     *** OUTPUT INFO ***
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,25) PEGMIN, PEGMAX, CONDCA
 25   FORMAT(4X, 'CL     = ', D12.5, /, 4X, 'CU     = ', D12.5, /,
     2       4X, 'CONDCA = ', D12.5, /)
C
      RETURN
      END
