      SUBROUTINE DPPCG(MATVEC,PCONDL,A,IA,X,B,N,Q,IQ,IPARAM,RPARAM,
     2   IWORK,R,H,W1,W2,W3,W4,D,E,IERROR)
C***BEGIN PROLOGUE  DPPCG
C***DATE WRITTEN   860715   (YYMMDD)
C***REVISION DATE  900208   (YYMMDD)
C***CATEGORY NO. D2B4
C***KEYWORDS  LINEAR SYSTEM,SPARSE,SYMMETRIC,POSITIVE DEFINITE,
C             ITERATIVE,PRECONDITION,CONJUGATE GRADIENTS,POLYNOMIAL,
C             CHEBYSHEV,ADAPTIVE
C***AUTHOR  ASHBY,STEVEN F., (UIUC)
C             UNIV. OF ILLINOIS
C             DEPT. OF COMPUTER SCIENCE
C             URBANA, IL 61801
C***AUTHOR  HOLST,MICHAEL J., (UIUC)
C             UNIV. OF ILLINOIS
C             DEPT. OF COMPUTER SCIENCE
C             URBANA, IL 61801
C           MANTEUFFEL,THOMAS A., (LANL)
C             LOS ALAMOS NATIONAL LABORATORY
C             MAIL STOP B265
C             LOS ALAMOS, NM 87545
C***PURPOSE  THIS SUBR SOLVES THE POLYNOMIAL PRECONDITIONED LINEAR
C            SYSTEM C(A)*AX = C(A)*B.   THE OPTIMAL (CHEBYSHEV)
C            PRECONDTIONING POLYNOMIAL C(A) (OF EVEN DEGREE) IS
C            DYNAMICALLY FOUND.  THE METHOD OF PRECONDITIONED CG IS 
C            USED, WITH C(A) AS THE PRECONDITIONER; A MUST BE SPD.
C
C***DEDCRIPTION
C
C--- ON ENTRY ---
C
C    MATVEC   EXTERNAL SUBROUTINE MATVEC(JOB,A,IA,W,X,Y,N)
C             THE USER MUST PROVIDE A SUBROUTINE HAVING THE SPECIFED
C             PARAMETER LIST.  THE SUBROUTINE MUST RETURN THE PRODUCT
C             (OR A RELATED COMPUTATION; SEE BELOW) Y=A*X, WHERE A IS 
C             THE COEFFICIENT MATRIX OF THE LINEAR SYSTEM.  THE MATRIX 
C             A IS REPRESENTED BY THE WORK ARRAYS A AND IA, DEDCRIBED
C             BELOW.  THE INTEGER PARAMETER JOB SPECIFIES THE PRODUCT 
C             TO BE COMPUTED:
C                  JOB=0    Y=A*X
C                  JOB=1    Y=AT*X
C                  JOB=2    Y=W - A*X
C                  JOB=3    Y=W - AT*X.  
C             IN THE ABOVE, AT DENOTES A-TRANSPOSE.  NOTE THAT
C             ONLY THE VALUES OF JOB=0,1 ARE REQUIRED FOR CGCODE.
C             ALL OF THE ROUTINES IN CGCODE REQUIRE JOB=0; THE
C             ROUTINES DCGNR, DCGNE, DPCGNR, AND DPCGNE ALSO REQUIRE
C             THE VALUE OF JOB=1.  (THE VALUES OF JOB=2,3 ARE NOT 
C             REQUIRED BY ANY OF THE ROUTINES IN CGCODE, BUT MAY BE 
C             REQUIRED BY OTHER ITERATIVE PACKAGES CONFORMING TO THE 
C             PROPOSED ITERATIVE STANDARD.)  THE PARAMETERS W,X,Y ARE 
C             ALL VECTORS OF LENGTH N.  THE ONLY PARAMETER THAT MAY BE 
C             CHANGED INSIDE THE ROUTINE IS Y.  MATVEC WILL USUALLY 
C             SERVE AS AN INTERFACE TO THE USER'S OWN MATRIX-VECTOR 
C             MULTIPLY SUBROUTINE.  
C             NOTE: MATVEC MUST BE DECLARED IN AN EXTERNAL STATEMENT 
C             IN THE CALLING PROGRAM.
C
C    PCONDL   EXTERNAL SUBROUTINE PCONDL(JOB,Q,IQ,W,X,Y,N)
C             PCONDL IMPLEMENTS A USER SUPPLIED LEFT-PRECONDITIONER.
C             IF PRECONDITIONING IS SPECIFIED BY THE USER, THEN THE 
C             USER MUST PROVIDE A SUBROUTINE HAVING THE SPECIFED
C             PARAMETER LIST.  THE SUBROUTINE MUST RETURN THE PRODUCT
C             (OR A RELATED COMPUTATION; SEE BELOW) Y=C*X, WHERE C
C             IS A PRECONDITIONING MATRIX.  THE MATRIX C IS 
C             REPRESENTED BY THE WORK ARRAYS Q AND IQ, DEDCRIBED
C             BELOW.  THE INTEGER PARAMETER JOB SPECIFIES THE PRODUCT 
C             TO BE COMPUTED:
C                  JOB=0    Y=C*X
C                  JOB=1    Y=CT*X
C                  JOB=2    Y=W - C*X
C                  JOB=3    Y=W - CT*X.  
C             IN THE ABOVE, CT DENOTES C-TRANSPOSE.  NOTE THAT
C             ONLY THE VALUES OF JOB=0,1 ARE REQUIRED FOR CGCODE.
C             THE ROUTINES DPCG, DPCGNR, DPCGNE, DPPCG, AND DPCGCA IN 
C             CGCODE REQUIRE JOB=0; THE ROUTINES DPCGNR AND DPCGNE ALSO 
C             REQUIRE THE VALUE OF JOB=1.  (THE VALUES OF JOB=2,3 ARE 
C             NOT REQUIRED BY ANY OF THE ROUTINES IN CGCODE, BUT MAY BE 
C             REQUIRED BY OTHER ITERATIVE PACKAGES CONFORMING TO THE 
C             PROPOSED ITERATIVE STANDARD.)  THE PARAMETERS W,X,Y ARE 
C             ALL VECTORS OF LENGTH N.  THE ONLY PARAMETER THAT MAY BE 
C             CHANGED INSIDE THE ROUTINE IS Y.  PCONDL WILL USUALLY 
C             SERVE AS AN INTERFACE TO THE USER'S OWN PRECONDITIONING
C             NOTE: PCONDL MUST BE DECLARED IN AN EXTERNAL STATEMENT 
C             IN THE CALLING PROGRAM.  IF NO PRE-CONDITIONING IS BEING 
C             DONE, PCONDL IS A DUMMY ARGUMENT.  
C
C    A        DBLE ARRAY ADDRESS.
C             THE BASE ADDRESS OF THE USER'S DBLE WORK ARRAY, USUALLY
C             THE MATRIX A.  SINCE A IS ONLY ACCESSED BY CALLS TO SUBR
C             MATVEC, IT MAY BE A DUMMY ADDRESS.
C
C    IA       INTEGER ARRAY ADDRESS.
C             THE BASE ADDRESS OF THE USER'S INTEGER WORK ARRAY.  THIS
C             USUALLY CONTAINS ADDITIONAL INFORMATION ABOUT A NEEDED BY
C             MATVEC.  SINCE IA IS ONLY ACCESSED BY CALLS TO MATVEC, IT
C             MAY BE A DUMMY ADDRESS.
C
C    X        DBLE(N).
C             THE INITIAL GUESS VECTOR, X0.
C             (ON EXIT, X IS OVERWRITTEN WITH THE APPROXIMATE SOLUTION
C             OF A*X=B.)
C
C    B        DBLE(N).
C             THE RIGHT-HAND SIDE VECTOR OF THE LINEAR SYSTEM AX=B.
C             NOTE: B IS CHANGED BY THE SOLVER.
C
C    N        INTEGER.
C             THE ORDER OF THE MATRIX A IN THE LINEAR SYSTEM AX=B.
C
C    Q        DBLE ARRAY ADDRESS.
C             THE BASE ADDRESS OF THE USER'S LEFT-PRECONDITIONING ARRAY, 
C             Q.  SINCE Q IS ONLY ACCESSED BY CALLS TO PCONDL, IT MAY BE 
C             A DUMMY ADDRESS.  IF NO LEFT-PRECONDITIONING IS BEING 
C             DONE, THIS IS A DUMMY ARGUMENT.
C
C    IQ       INTEGER ARRAY ADDRESS.
C             THE BASE ADDRESS OF AN INTEGER WORK ARRAY ASSOCIATED WITH
C             Q.  THIS PROVIDES THE USER WITH A WAY OF PASSING INTEGER
C             INFORMATION ABOUT Q TO PCONDL.  SINCE IQ IS ONLY ACCESSED
C             BY CALLS TO PCONDL, IT MAY BE A DUMMY ADDRESS.  IF NO 
C             LEFT-PRECONDITIONING IS BEING DONE, THIS IS A DUMMY 
C             ARGUMENT.
C
C    IPARAM   INTEGER(40).
C             AN ARRAY OF INTEGER INPUT PARAMETERS:
C                NOTE: IPARAM(1) THROUGH IPARAM(10) ARE MANDATED BY THE 
C                PROPOSED STANDARD; IPARAM(11) THROUGH IPARAM(30) ARE
C                RESERVED FOR EXPANSION OF THE PROPOSED STANDARD; 
C                IPARAM(31) THROUGH IPARAM(34) ARE ADDITIONAL 
C                PARAMETERS, SPECIFIC TO CGCODE.
C
C             IPARAM(1) = NIPAR 
C             LENGTH OF THE IPARAM ARRAY.  
C
C             IPARAM(2) = NRPAR 
C             LENGTH OF THE RPARAM ARRAY.  
C
C             IPARAM(3) = NIWK 
C             LENGTH OF THE IWORK ARRAY.  
C
C             IPARAM(4) = NRWK 
C             LENGTH OF THE RWORK ARRAY.  
C
C             IPARAM(5) = IOUNIT
C             IF (IOUNIT > 0) THEN ITERATION INFORMATION (AS 
C             SPECIFIED BY IOLEVL; SEE BELOW) IS SENT TO UNIT=IOUNIT,
C             WHICH MUST BE OPENED IN THE CALLING PROGRAM.  
C             IF (IOUNIT <= 0) THEN THERE IS NO OUTPUT.
C
C             IPARAM(6) = IOLEVL 
C             SPECIFIES THE AMOUNT AND TYPE OF INFORMATION TO BE 
C             OUTPUT IF (IOUNIT > 0):  
C                IOLEVL = 0   OUTPUT ERROR MESSAGES ONLY
C                IOLEVL = 1   OUTPUT INPUT PARAMETERS AND LEVEL 0 INFO
C                IOLEVL = 2   OUTPUT STPTST (SEE BELOW) AND LEVEL 1 INFO
C                IOLEVL = 3   OUTPUT LEVEL 2 INFO AND MORE DETAILS
C
C             IPARAM(7) = IPCOND 
C             PRECONDITIONING FLAG, SPECIFIED AS:
C                IPCOND = 0   NO PRECONDITIONING
C                IPCOND = 1   LEFT PRECONDITIONING
C                IPCOND = 2   RIGHT PRECONDITIONING 
C                IPCOND = 3   BOTH LEFT AND RIGHT PRECONDITIONING 
C             NOTE:  RIGHT PRECONDITIONING IS A MANDATED OPTION OF THE 
C             PROPOSED STANDARD, BUT NOT IMPLEMENTED IN CGCODE.
C
C             IPARAM(8) = ISTOP
C             STOPPING CRITERION FLAG, INTERPRETED AS:
C                ISTOP = 0  ||E||/||E0||      <= ERRTOL  (DEFAULT)
C                ISTOP = 1  ||R||             <= ERRTOL 
C                ISTOP = 2  ||R||/||B||       <= ERRTOL
C                ISTOP = 3  ||C*R||           <= ERRTOL
C                ISTOP = 4  ||C*R||/||C*B||   <= ERRTOL
C             WHERE E=ERROR, R=RESIDUAL, B=RIGHT HAND SIDE OF A*X=B, 
C             AND C IS THE PRECONDITIONING MATRIX OR PRECONDITIONING 
C             POLYNOMIAL (OR BOTH.) 
C             NOTE: IF ISTOP=0 IS SELECTED BY THE USER, THEN ERRTOL 
C             IS THE AMOUNT BY WHICH THE INITIAL ERROR IS TO BE 
C             REDUCED.  BY ESTIMATING THE CONDITION NUMBER OF THE 
C             ITERATION MATRIX, THE CODE ATTEMPTS TO GUARANTEE THAT 
C             THE FINAL RELATIVE ERROR IS .LE. ERRTOL.  SEE THE LONG 
C             DEDCRIPTION BELOW FOR DETAILS.
C
C             IPARAM(9) = ITMAX
C             THE MAXIMUM NUMBER OF ITERATIVE STEPS TO BE TAKEN.
C             IF SOLVER IS UNABLE TO SATISFY THE STOPPING CRITERION 
C             WITHIN ITMAX ITERATIONS, IT RETURNS TO THE CALLING
C             PROGRAM WITH IERROR=-1000.
C
C             IPARAM(31) = ICYCLE
C             THE FREQUENCY WITH WHICH A CONDITION NUMBER ESTIMATE IS
C             COMPUTED; SEE THE LONG DEDCRIPTION BELOW.
C
C             IPARAM(32) = NCE
C             THE MAXIMUM NUMBER OF CONDITION NUMBER ESTIMATES TO BE
C             COMPUTED.  IF NCE = 0 NO ESTIMATES ARE COMPUTED.  SEE
C             THE LONG DEDCRIPTION BELOW.
C
C             IPARAM(34) = NDEG
C             WHEN USING THE CONJUGATE GRADIENT ROUTINES DPPCG AND
C             DPCGCA, NDEG SPECIFIES THE DEGREE OF THE PRECONDITIONING 
C             POLYNOMIAL TO BE USED IN THE ADAPTIVE POLYNOMIAL 
C             PRECONDITIONING ROUTINES.
C
C             NOTE:  KMAX = ICYCLE*NCE IS THE ORDER OF THE LARGEST
C             ORTHOGONAL SECTION OF C*A USED TO COMPUTE A CONDITION
C             NUMBER ESTIMATE.  THIS ESTIMATE IS ONLY USED IN THE
C             STOPPING CRITERION.  AS SUCH, KMAX SHOULD BE MUCH LESS
C             THAN N.  OTHERWISE THE CODE WILL HAVE EXCESSIVE STORAGE
C             AND WORK REQUIREMENTS.
C
C    RPARAM   DBLE(40).
C             AN ARRAY OF DBLE INPUT PARAMETERS:
C                NOTE: RPARAM(1) AND RPARAM(2) ARE MANDATED BY THE 
C                PROPOSED STANDARD; RPARAM(3) THROUGH RPARAM(30) ARE
C                RESERVED FOR EXPANSION OF THE PROPOSED STANDARD;
C                RPARAM(31) THROUGH RPARAM(34) ARE ADDITIONAL 
C                PARAMETERS, SPECIFIC TO CGCODE.
C
C             RPARAM(1) = ERRTOL
C             USER PROVIDED ERROR TOLERANCE; SEE ISTOP ABOVE, AND THE
C             LONG DEDCRIPTION BELOW.
C
C             RPARAM(31) = CONDES
C             AN INITIAL ESTIMATE FOR THE COND NUMBER OF THE ITERATION
C             MATRIX; SEE THE INDIVIDUAL SUBROUTINE'S PROLOGUE. AN 
C             ACCEPTABLE INITIAL VALUE IS 1.0.
C
C             RPARAM(32) = AA     
C             INITIAL ESTIMATE OF THE SMALLEST EIGENVALUE OF THE 
C             SYSTEM MATRIX.  WHEN USING THE CONJUGATE GRADIENT
C             ROUTINES DPPCG AND DPCGCA, AA IS USED IN THE ADAPTIVE 
C             POLYNOMIAL PRECONDITIONING ROUTINES FOR FORMING THE 
C             OPTIMAL PRECONDITIONING POLYNOMIAL.
C
C             RPARAM(33) = BB
C             INITIAL ESTIMATE OF THE LARGEST EIGENVALUE OF THE 
C             SYSTEM MATRIX.  WHEN USING THE CONJUGATE GRADIENT
C             ROUTINES DPPCG AND DPCGCA, BB IS USED IN THE ADAPTIVE 
C             POLYNOMIAL PRECONDITIONING ROUTINES FOR FORMING THE 
C             OPTIMAL PRECONDITIONING POLYNOMIAL.
C
C    R,H      DBLE(N), DBLE(N).
C             WORK ARRAYS FOR THE CG ITERATION.
C
C    W1,W2    DBLE(N), DBLE(N).
C             WORK ARRAYS FOR EXECUTING THE POLYNOMIAL PRECONDITIONING.
C 
C    W3,W4    DBLE(N), DBLE(N).
C             WORK ARRAYS FOR EXECUTING THE INNER PRECONDITIONING.  THESE
C             ARE DUMMY ARGUMENTS IF IPCOND=0.
C
C    D,E      DBLE(ICYCLE*NCE + 1), DBLE(ICYCLE*NCE + 1).
C    IWORK    INTEGER(ICYCLE*NCE).
C             WORK ARRAYS FOR COMPUTING CONDITION NUMBER ESTIMATES.
C             IF NCE=0 THESE MAY BE DUMMY ADDRESSES.
C
C--- ON RETURN ---
C
C    IPARAM   THE FOLLOWING ITERATION INFO IS RETURNED VIA THIS ARRAY:
C
C             IPARAM(10) = ITERS
C             THE NUMBER OF ITERATIONS TAKEN.  IF IERROR=0, THEN X_ITERS
C             SATISFIES THE SPECIFIED STOPPING CRITERION.  IF 
C             IERROR=-1000, CGCODE WAS UNABLE TO CONVERGE WITHIN ITMAX 
C             ITERATIONS, AND X_ITERS IS CGCODE'S BEST APPROXIMATION TO 
C             THE SOLUTION OF A*X=B.
C
C    RPARAM   THE FOLLOWING ITERATION INFO IS RETURNED VIA THIS ARRAY:
C
C             RPARAM(2) = STPTST
C             FINAL QUANTITY USED IN THE STOPPING CRITERION; SEE ISTOP
C             ABOVE, AND THE LONG DEDCRIPTION BELOW.
C
C             RPARAM(31) = CONDES
C             CONDITION NUMBER ESTIMATE; FINAL ESTIMATE USED IN THE 
C             STOPPING CRITERION; SEE ISTOP ABOVE, AND THE LONG 
C             DEDCRIPTION BELOW.
C
C             RPARAM(34) = DCRLRS
C             THE SCALED RELATIVE RESIDUAL USING THE LAST COMPUTED 
C             RESIDUAL.
C
C    X        THE COMPUTED SOLUTION OF THE LINEAR SYSTEM AX=B.
C
C    IERROR   INTEGER.
C             ERROR FLAG (NEGATIVE ERRORS ARE FATAL):
C             (BELOW, A=SYSTEM MATRIX, Q=LEFT PRECONDITIONING MATRIX.)
C             IERROR =  0      NORMAL RETURN: ITERATION CONVERGED
C             IERROR =  -1000  METHOD FAILED TO CONVERGE IN ITMAX STEPS
C             IERROR = +-2000  ERROR IN USER INPUT
C             IERROR = +-3000  METHOD BREAKDOWN
C             IERROR =  -6000  A DOES NOT SATISTY ASSUMPTIONS OF METHOD
C             IERROR =  -7000  Q DOES NOT SATISTY ASSUMPTIONS OF METHOD
C
C***LONG DEDCRIPTION
C
C    (IF IPCOND=1 THE FOLLOWING REMARKS PERTAIN TO C*A RATHER THAN A.)
C
C    DPPCG IMPLEMENTS THE PRECONDITIONED CONJUGATE GRADIENT METHOD,
C    WITH A DYNAMICALLY COMPUTED OPTIMAL POLYNOMIAL PRECONDITIONER, 
C    USING THE OMIN ALGORITHM GIVEN BY:
C 
C                   R0 = B - A*X0
C                   H0 = C(A)*R0
C                   P0 = H0
C                   AP = A*P
C                   ALPHA = <R,H>/<AP,P>
C                   XNEW = X + ALPHA*P
C                   RNEW = R - ALPHA*AP
C                   HNEW = C(A)*R
C                   BETA = <RNEW,HNEW>/<R,H>
C                   PNEW = HNEW + BETA*P
C
C    THIS ALGORITHM IS GUARANTEED TO CONVERGE IF BOTH A AND C(A) ARE
C    SYMMETRIC POSITIVE DEFINITE.   MATHEMATICALLY, IF C(A)*A HAS M
C    DISTINCT EGVALS, THE ALGORITHM WILL CONVERGE IN AT MOST M STEPS.
C    AT EACH STEP THE ALGORITHM MINIMIZES THE A-NORM OF THE ERROR. THAT
C    IS, IT MINIMIZES <A*E,E>.  
C
C    THE CODE DYNAMICALLY SEEKS THE OPTIMAL PRECONDITIONING POLY, C(A).
C    THIS IS DONE AS FOLLOWS.  EVERY ICYCLE STEPS ESTIMATES FOR THE MIN
C    (CL) AND MAX (CU) EGVALS OF C(A)A ARE COMPUTED FROM AN ORTHOGONAL
C    SECTION OF C(A)A; SEE THE REFERENCES BELOW.  (THE LARGEST POSSIBLE
C    ORDER IS ICYCLE*NCE, WHERE NCE IS THE MAXIMUM NUMBER OF ADAPTIVE
C    CALLS.)  ONCE CL AND CU ARE COMPUTED, NEW ESTIMATES FOR AA AND BB
C    ARE DETERMINED.  IF THE NEW CONVERGENCE FACTOR IS SMALLER THAN THE
C    CURRENT CF, THE ITERATION IS RESTARTED, USING THE NEW AA AND BB.
C    OTHERWISE THE ITERATION CONTINUES.  
C
C    WHEN THE USER SELECTS THE STOPPING CRITERION OPTION ISTOP=0, THEN
C    THE CODE STOPS WHEN COND(C(A)A)*(HNORM/H0NORM) .LE. ERRTOL, THEREBY
C    ATTEMPTING TO GUARANTEE THAT (FINAL RELATIVE ERROR) .LE. ERRTOL.
C    A NEW COND(C(A)A) ESTIMATE IS COMPUTED FROM CL AND CU.  IF NCE=0,
C    NO CONDITION ESTIMATES ARE COMPUTED.  IN THIS CASE, THE CODE STOPS
C    WHEN COND0*(HNORM/H0NORM) .LE. ERRTOL, WHERE COND0 IS THE CONDITION
C    NUMBER ESTIMATE FOR C(A)A OBTAINED FROM THE INITIAL AA AND BB.
C
C    THIS STOPPING CRITERION WAS IMPLEMENTED BY A.J. ROBERTSON, III
C    (DEPT. OF MATHEMATICS, UNIV. OF COLORADO AT DENVER).  QUESTIONS
C    MAY BE DIRECTED TO HIM OR TO ONE OF THE AUTHORS.
C
C    DPPCG IS ONE ROUTINE IN A PACKAGE OF CG CODES; THE OTHERS ARE:
C
C    DCGDRV : AN INTERFACE TO ANY ROUTINE IN THE PACKAGE
C    DCG    : CONJUGATE GRADIENTS ON A, A SPD (CGHS)
C    DCR    : CONJUGATE RESIDUALS ON A, A SPD (CR)
C    DCRIND : CR ON A, A SYMMETRIC (CRIND)
C    DPCG   : PRECONITIONED CG ON A, A AND C SPD (PCG)
C    DCGNR  : CGHS ON AT*A, A ARBITRARY (CGNR)
C    DCGNE  : CGHS ON A*AT, A ARBITRARY (CGNE)
C    DPCGNR : CGNR ON A*C, A AND C ARBITRARY (PCGNR)
C    DPCGNE : CGNE ON C*A, A AND C ARBITRARY (PCGNE)
C    DPPCG  : POLYNOMIAL PCG ON A, A AND C SPD (PPCG)
C    DPCGCA : CGHS ON C(A)*A, A AND C SPD (PCGCA)
C
C***REFERENCES  HOWARD C. ELMAN, "ITERATIVE METHODS FOR LARGE, SPARSE,
C                 NONSYMMETRIC SYSTEMS OF LINEAR EQUATIONS", YALE UNIV.
C                 DCS RESEARCH REPORT NO. 229 (APRIL 1982).
C               VANCE FABER AND THOMAS MANTEUFFEL, "NECESSARY AND
C                 SUFFICIENT CONDITIONS FOR THE EXISTENCE OF A
C                 CONJUGATE GRADIENT METHODS", SIAM J. NUM ANAL 21(2),
C                 PP. 352-362, 1984.
C               S. ASHBY, T. MANTEUFFEL, AND P. SAYLOR, "A TAXONOMY FOR
C                 CONJUGATE GRADIENT METHODS", SIAM J. NUM ANAL 27(6),
C                 PP. 1542-1568, 1990.
C               S. ASHBY, M. HOLST, T. MANTEUFFEL, AND P. SAYLOR,
C                 THE ROLE OF THE INNER PRODUCT IN STOPPING CRITERIA
C                 FOR CONJUGATE GRADIENT ITERATIONS", BIT 41(1),
C                 PP. 26-53, 2001.
C               M. HOLST, "CGCODE: SOFTWARE FOR SOLVING LINEAR SYSTEMS
C                 WITH CONJUGATE GRADIENT METHODS", M.S. THESIS, UNIV. 
C                 OF ILLINOIS DCS RESEARCH REPORT (MAY 1990).
C               S. ASHBY, "POLYNOMIAL PRECONDITIONG FOR CONJUGATE 
C                 GRADIENT METHODS", PH.D. THESIS, UNIV. OF ILLINOIS
C                 DCS RESEARCH REPORT NO. R-87-1355 (DECEMBER 1987).
C               S. ASHBY, M. SEAGER, "A PROPOSED STANDARD FOR ITERATIVE
C                 LINEAR SOLVERS", LAWRENCE LIVERMORE NATIONAL 
C                 LABORATORY REPORT (TO APPEAR).
C
C***ROUTINES CALLED  DDPCHB,DCBFIX,DONEST,D1MACH,DAXPY,DCGCHK,DCKCHB,
C                    DDOT,DNRM2
C***END PROLOGUE  DPPCG
C
C     *** DECLARATIONS ***
      IMPLICIT  DOUBLE PRECISION(A-H,O-Z)
      EXTERNAL  MATVEC,PCONDL
      DIMENSION X(N),B(N),R(N),H(N),W1(N),W2(N),W3(*),W4(*)
      DIMENSION IPARAM(*),RPARAM(*),D(*),E(*),IWORK(*)
C
C***FIRST EXECUTABLE STATEMENT  DPPCG
 1    CONTINUE
C
C     *** CHECK THE INPUT PARAMETERS ***
      IOUNIT = IPARAM(5)
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,5)
 5    FORMAT(' THE METHOD IS POLYNOMIAL PPCG', /)
      CALL DCGCHK(IPARAM,RPARAM,N)
      CALL DCKCHB(IPARAM,RPARAM,PEGMIN,PEGMAX,CONDCA)
      IF (IOUNIT .GT. 0) THEN
         IF (IPCOND .EQ. 0) WRITE(IOUNIT,10)
         IF (IPCOND .EQ. 1) WRITE(IOUNIT,11)
 10      FORMAT(' RESID  = DSQRT(<C(A)R,R>)', /,
     2          ' RELRSD = RESID / INITIAL RESID', /)
 11      FORMAT(' RESID  = DSQRT(<C(C*A)C*R,R>)', /,
     2          ' RELRSD = RESID / INITIAL RESID', /)
      END IF
C
C     *** INITIALIZE INPUT PARAMETERS ***
      IPCOND = IPARAM(7)
      ISTOP  = IPARAM(8)
      ITMAX  = IPARAM(9)
      ICYCLE = IPARAM(31)
      NCE    = IPARAM(32)
      KMAX   = ICYCLE*NCE
      NDEG   = IPARAM(34)
      NCHB   = NDEG+1
      ERRTOL = RPARAM(1)
      AA     = RPARAM(32)
      BB     = RPARAM(33)
C
C     *** INITIALIZE ITERS, E(1), ERRED, R ***
      E(1)   = 0.0D0
      ERRED  = 1.0D0
      CALL MATVEC(0,A,IA,WDUMMY,X,R,N)
      DO 15 I = 1, N
         R(I) = B(I) - R(I)
 15   CONTINUE
      ITERS  = 0
C
C     *** INITIALIZE ITERATION PARAMETERS ***
 20   KSTEP  = 0
      D(1)   = 0.0D0
      ERRTOL = ERRTOL*ERRED
C
C     *** COMPUTE STOPPING CRITERION DENOMINATOR ***
      DENOM = 1.0D0
      IF (ISTOP .EQ. 2) DENOM = DNRM2(N,B,1)
      IF ((ISTOP .EQ. 0) .OR. (ISTOP .EQ. 4)) THEN
         IF (IPCOND .EQ. 0) THEN
            CALL DCBFIX(MATVEC,PCONDL,A,IA,Q,IQ,IPCOND,
     2                  AA,BB,NCHB,B,H,W1,W2,W3,N)
            DENOM = DSQRT(DDOT(N,B,1,H,1))
         ELSE
            CALL PCONDL(0,Q,IQ,WDUMMY,B,W4,N)
            CALL DCBFIX(MATVEC,PCONDL,A,IA,Q,IQ,IPCOND,
     2                  AA,BB,NCHB,W4,H,W1,W2,W3,N)
            DENOM = DSQRT(DDOT(N,B,1,H,1))
         END IF
      END IF
C
C     *** TELL MDSTOP WHETHER OR NOT I AM SUPPLYING THE STOPPING QUANTITY ***
      IF ((ISTOP .EQ. 1) .OR. (ISTOP .EQ. 2)) THEN
         IDO = 0
      ELSE
         IDO = 1
      ENDIF
C
C     *** COMPUTE INITIAL H ***
      IF (IPCOND .EQ. 0) THEN
         CALL DCBFIX(MATVEC,PCONDL,A,IA,Q,IQ,IPCOND,
     2               AA,BB,NCHB,R,B,W1,W2,W3,N)
      ELSE
         CALL PCONDL(0,Q,IQ,WDUMMY,R,W4,N)
         CALL DCBFIX(MATVEC,PCONDL,A,IA,Q,IQ,IPCOND,
     2               AA,BB,NCHB,W4,B,W1,W2,W3,N)
      END IF
      S0NORM = DSQRT(DDOT(N,R,1,B,1))
      SNORM = S0NORM
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,25) S0NORM
 25   FORMAT(' INITIAL RESID   = ', D12.5, /)
C
C     *** CHECK THE INITIAL RESIDUAL ***
      JSTOP = MDSTOP(ISTOP,ITERS,ITMAX,ERRTOL,STPTST,IERROR,
     2              R,SDUMM,ZDUMM,N,RNDUMM,S0NORM,S0NORM,
     3              DENOM,CONDCA,IDO)
      IF (JSTOP .EQ. 1) GOTO 90
C
C     *** UPDATE ITERS, KSTEP AND COMPUTE A*P ***
 30   ITERS = ITERS + 1
      KSTEP = KSTEP + 1
      CALL MATVEC(0,A,IA,WDUMMY,B,H,N)
C
C     *** COMPUTE NEW X ***
      ALPHA =  SNORM**2 / DDOT(N,H,1,B,1)
      CALL DAXPY(N,ALPHA,B,1,X,1)
C
C     *** COMPUTE NEW R AND H ***
      CALL DAXPY(N,-ALPHA,H,1,R,1)
      IF (IPCOND .EQ. 0) THEN
         CALL DCBFIX(MATVEC,PCONDL,A,IA,Q,IQ,IPCOND,
     2               AA,BB,NCHB,R,H,W1,W2,W3,N)
      ELSE
         CALL PCONDL(0,Q,IQ,WDUMMY,R,W4,N)
         CALL DCBFIX(MATVEC,PCONDL,A,IA,Q,IQ,IPCOND,
     2               AA,BB,NCHB,W4,H,W1,W2,W3,N)
      END IF
      OLDSNM = SNORM
      SNORM = DSQRT(DDOT(N,R,1,H,1))
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,35) ITERS, SNORM, SNORM/S0NORM
 35   FORMAT(' ITERS = ',I5,4X, 'RESID = ',D12.5,4X, 'RELRSD = ',D12.5)
C
C     *** TEST TO HALT ***
      JSTOP = MDSTOP(ISTOP,ITERS,ITMAX,ERRTOL,STPTST,IERROR,
     2              R,SDUMM,ZDUMM,N,RNDUMM,SNORM,SNORM,
     3              DENOM,CONDCA,IDO)
      IF (JSTOP .EQ. 1) GOTO 90
C
C     *** CONDITION ESTIMATE ***
      BETA = (SNORM/OLDSNM)**2
      IF (ITERS .LE. KMAX) THEN
C        *** SAVE PARAMETERS ***
         KSP1 = KSTEP + 1
         RALPHA = 1.0D0/ALPHA
         D(KSTEP) = D(KSTEP) + RALPHA
         D(KSP1) = BETA*RALPHA
         E(KSP1) = -DSQRT(BETA)*RALPHA
         IF (MOD(ITERS,ICYCLE) .EQ. 0) THEN
            ERRED = S0NORM/(SNORM*CONDCA)
            IF (IOUNIT .GT. 0) WRITE(IOUNIT,40)
 40         FORMAT(/, ' NEW ESTIMATES FOR C(A)*A:')
            CALL DONEST(IOUNIT,D,E,W1,W2,IWORK,
     2                  KSTEP,PEGMIN,PEGMAX,CONDCA)
            EREPS = ERRTOL*ERRED
            CALL DDPCHB(IOUNIT,AA,BB,NCHB,PEGMIN,
     2                  PEGMAX,CONDCA,EREPS,IADAPT)
            IF (IADAPT .EQ. 2) GOTO 20
         END IF
      END IF
C
C     *** COMPUTE NEW P ***
      DO 45 I = 1, N
         B(I) = H(I) + BETA*B(I)
 45   CONTINUE
C
C     *** RESUME CG ITERATION ***
      GOTO 30
C
C     *** RETURN UPDATED PARAMETERS ***
 90   IPARAM(10) = ITERS
      RPARAM(2)  = STPTST
      RPARAM(31) = CONDCA
      RPARAM(34) = SNORM/S0NORM
      RPARAM(32) = AA
      RPARAM(33) = BB
C
      RETURN
      END
