      SUBROUTINE SCRIND(MATVEC,A,IA,X,B,N,IPARAM,RPARAM,IWORK,
     2   R,AAP,AP,POLD,APOLD,D,E,CNDWK,IERROR)
C***BEGIN PROLOGUE  SCRIND
C***DATE WRITTEN   870715   (YYMMDD)
C***REVISION DATE  900210   (YYMMDD)
C***CATEGORY NO. D2B4
C***KEYWORDS  LINEAR SYSTEM,SPARSE,SYMMETRIC,INDEFINITE,ITERATIVE,
C             CONJUGATE GRADIENTS,CONJUGATE RESIDUALS,ODIR,OMIN
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
C***PURPOSE  THIS SUBROUTINE SOLVES THE SYMMETRIC (POSSIBLY INDEFINITE)
C            LINEAR SYSTEM AX=B.  THE METHOD OF CONJUGATE RESIDUALS IS
C            USED WITH A HYBRID ODIR/OMIN IMPLEMENTATION.
C***DESCRIPTION
C
C--- ON ENTRY ---
C
C    MATVEC   EXTERNAL SUBROUTINE MATVEC(JOB,A,IA,W,X,Y,N)
C             THE USER MUST PROVIDE A SUBROUTINE HAVING THE SPECIFED
C             PARAMETER LIST.  THE SUBROUTINE MUST RETURN THE PRODUCT
C             (OR A RELATED COMPUTATION; SEE BELOW) Y=A*X, WHERE A IS 
C             THE COEFFICIENT MATRIX OF THE LINEAR SYSTEM.  THE MATRIX 
C             A IS REPRESENTED BY THE WORK ARRAYS A AND IA, DESCRIBED
C             BELOW.  THE INTEGER PARAMETER JOB SPECIFIES THE PRODUCT 
C             TO BE COMPUTED:
C                  JOB=0    Y=A*X
C                  JOB=1    Y=AT*X
C                  JOB=2    Y=W - A*X
C                  JOB=3    Y=W - AT*X.  
C             IN THE ABOVE, AT DENOTES A-TRANSPOSE.  NOTE THAT
C             ONLY THE VALUES OF JOB=0,1 ARE REQUIRED FOR CGCODE.
C             ALL OF THE ROUTINES IN CGCODE REQUIRE JOB=0; THE
C             ROUTINES SCGNR, SCGNE, SPCGNR, AND SPCGNE ALSO REQUIRE
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
C    A        REAL ARRAY ADDRESS.
C             THE BASE ADDRESS OF THE USER'S REAL WORK ARRAY, USUALLY
C             THE MATRIX A.  SINCE A IS ONLY ACCESSED BY CALLS TO SUBR
C             MATVEC, IT MAY BE A DUMMY ADDRESS.
C
C    IA       INTEGER ARRAY ADDRESS.
C             THE BASE ADDRESS OF THE USER'S INTEGER WORK ARRAY.  THIS
C             USUALLY CONTAINS ADDITIONAL INFORMATION ABOUT A NEEDED BY
C             MATVEC.  SINCE IA IS ONLY ACCESSED BY CALLS TO MATVEC, IT
C             MAY BE A DUMMY ADDRESS.
C
C    X        REAL(N).
C             THE INITIAL GUESS VECTOR, X0.
C             (ON EXIT, X IS OVERWRITTEN WITH THE APPROXIMATE SOLUTION
C             OF A*X=B.)
C
C    B        REAL(N).
C             THE RIGHT-HAND SIDE VECTOR OF THE LINEAR SYSTEM AX=B.
C             NOTE: B IS CHANGED BY THE SOLVER.
C
C    N        INTEGER.
C             THE ORDER OF THE MATRIX A IN THE LINEAR SYSTEM AX=B.
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
C             DESCRIPTION BELOW FOR DETAILS.
C
C             IPARAM(9) = ITMAX
C             THE MAXIMUM NUMBER OF ITERATIVE STEPS TO BE TAKEN.
C             IF SOLVER IS UNABLE TO SATISFY THE STOPPING CRITERION 
C             WITHIN ITMAX ITERATIONS, IT RETURNS TO THE CALLING
C             PROGRAM WITH IERROR=-1000.
C
C             IPARAM(31) = ICYCLE
C             THE FREQUENCY WITH WHICH A CONDITION NUMBER ESTIMATE IS
C             COMPUTED; SEE THE LONG DESCRIPTION BELOW.
C
C             IPARAM(32) = NCE
C             THE MAXIMUM NUMBER OF CONDITION NUMBER ESTIMATES TO BE
C             COMPUTED.  IF NCE = 0 NO ESTIMATES ARE COMPUTED.  SEE
C             THE LONG DESCRIPTION BELOW.
C
C             NOTE:  KMAX = ICYCLE*NCE IS THE ORDER OF THE LARGEST
C             ORTHOGONAL SECTION OF C*A USED TO COMPUTE A CONDITION
C             NUMBER ESTIMATE.  THIS ESTIMATE IS ONLY USED IN THE
C             STOPPING CRITERION.  AS SUCH, KMAX SHOULD BE MUCH LESS
C             THAN N.  OTHERWISE THE CODE WILL HAVE EXCESSIVE STORAGE
C             AND WORK REQUIREMENTS.
C
C    RPARAM   REAL(40).
C             AN ARRAY OF REAL INPUT PARAMETERS:
C                NOTE: RPARAM(1) AND RPARAM(2) ARE MANDATED BY THE 
C                PROPOSED STANDARD; RPARAM(3) THROUGH RPARAM(30) ARE
C                RESERVED FOR EXPANSION OF THE PROPOSED STANDARD;
C                RPARAM(31) THROUGH RPARAM(34) ARE ADDITIONAL 
C                PARAMETERS, SPECIFIC TO CGCODE.
C
C             RPARAM(1) = ERRTOL
C             USER PROVIDED ERROR TOLERANCE; SEE ISTOP ABOVE, AND THE
C             LONG DESCRIPTION BELOW.
C
C             RPARAM(31) = CONDES
C             AN INITIAL ESTIMATE FOR THE COND NUMBER OF THE ITERATION
C             MATRIX; SEE THE INDIVIDUAL SUBROUTINE'S PROLOGUE. AN 
C             ACCEPTABLE INITIAL VALUE IS 1.0.
C
C    R        REAL(N).
C             WORK ARRAY OF LENGTH .GE. N.
C
C    AAP      REAL(N).
C             WORK ARRAY OF LENGTH .GE. N.
C
C    AP       REAL(N).
C             WORK ARRAY OF LENGTH .GE. N.
C
C    POLD     REAL(N).
C             WORK ARRAY OF LENGTH .GE. N.
C
C    APOLD    REAL(N).
C             WORK ARRAY OF LENGTH .GE. N.
C
C    D,E      REAL(ICYCLE*NCE + 1), REAL(ICYCLE*NCE + 1).
C    CNDWK    REAL(2*ICYCLE*NCE).
C    IWORK    INTEGER(ICYCLE*NCE).
C             WORK ARRAYS FOR COMPUTING CONDITION NUMBER ESTIMATES.
C             IF NCE = 0 THESE MAY BE DUMMY ADDRESSES.
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
C             ABOVE, AND THE LONG DESCRIPTION BELOW.
C
C             RPARAM(31) = CONDES
C             CONDITION NUMBER ESTIMATE; FINAL ESTIMATE USED IN THE 
C             STOPPING CRITERION; SEE ISTOP ABOVE, AND THE LONG 
C             DESCRIPTION BELOW.
C
C             RPARAM(34) = SCRLRS
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
C***LONG DESCRIPTION
C
C    SCRIND IMPLEMENTS THE CONJUGATE RESIDUAL METHOD FOR SYMMETRIC
C    MATRICES, USING A HYBRID ODIR/OMIN ALGORITHM GIVEN BY:
C 
C                   P0 = R0
C                   ALPHA = <A*P,R>/<A*P,A*P>
C                   XNEW = X + ALPHA*P
C                   RNEW = R - ALPHA*(A*P)
C                   IF (ALPHA < EPSILON) THEN
C                      IF (ALPHA_OLD < EPSILON) THEN
C                         C = 1.0
C                      ELSE
C                         C = -1.0 / ALPHA_OLD
C                      ENDIF
C                      SIGMA = C * <A*P,A*P>/<A*POLD,A*POLD>
C                      GAMMA = <A*A*P,A*P>/<A*P,A*P>
C                      PNEW = A*P - GAMMA*P - SIGMA*POLD
C                   ELSE
C                      BETA = <A*RNEW,A*P>/<A*P,A*P>
C                      PNEW = RNEW - BETA*P
C                   ENDIF
C
C    THIS ALGORITHM IS GUARANTEED TO CONVERGE FOR SYMMETRIC MATRICES.  
C    MATHEMATICALLY, IF THE MATRIX HAS M DISTINCT EIGENVALUES, THE 
C    ALGORITHM WILL CONVERGE IN AT MOST M STEPS.  AT EACH STEP THE 
C    ALGORITHM MINIMIZES THE 2-NORM OF THE RESIDUAL.
C
C    WHEN THE USER SELECTS THE STOPPING CRITERION OPTION ISTOP=0, THEN
C    THE CODE STOPS WHEN  COND(A)*(RNORM/R0NORM) .LE. ERRTOL, THEREBY
C    ATTEMPTING TO GUARANTEE THAT (FINAL RELATIVE ERROR) .LE. ERRTOL.
C    A NEW ESTIMATE FOR COND(A) IS COMPUTED EVERY ICYCLE STEPS. THIS
C    IS DONE BY COMPUTING THE MIN AND MAX EIGENVALUES OF AN ORTHOGONAL 
C    SECTION OF A.  THE LARGEST ORTHOG SECTION HAS ORDER ICYCLE*NCE,
C    WHERE NCE IS THE MAXIMUM NUMBER OF CONDITION ESTIMATES.  IF NCE=0,
C    NO CONDITION ESTIMATES ARE COMPUTED.  IN THIS CASE, THE CODE STOPS
C    WHEN RNORM/R0NORM .LE. ERRTOL.  (ALSO SEE THE PROLOGUE TO SCGDRV.)
C
C    THIS STOPPING CRITERION WAS IMPLEMENTED BY A.J. ROBERTSON, III
C    (DEPT. OF MATHEMATICS, UNIV. OF COLORADO AT DENVER).  QUESTIONS
C    MAY BE DIRECTED TO HIM OR TO ONE OF THE AUTHORS.
C
C    SCRIND IS ONE ROUTINE IN A PACKAGE OF CG CODES; THE OTHERS ARE:
C
C    SCGDRV : AN INTERFACE TO ANY ROUTINE IN THE PACKAGE
C    SCG    : CONJUGATE GRADIENTS ON A, A SPD (CGHS)
C    SCR    : CONJUGATE RESIDUALS ON A, A SPD (CR)
C    SCRIND : CR ON A, A SYMMETRIC (CRIND)
C    SPCG   : PRECONITIONED CG ON A, A AND C SPD (PCG)
C    SCGNR  : CGHS ON AT*A, A ARBITRARY (CGNR)
C    SCGNE  : CGHS ON A*AT, A ARBITRARY (CGNE)
C    SPCGNR : CGNR ON A*C, A AND C ARBITRARY (PCGNR)
C    SPCGNE : CGNE ON C*A, A AND C ARBITRARY (PCGNE)
C    SPPCG  : POLYNOMIAL PCG ON A, A AND C SPD (PPCG)
C    SPCGCA : CGHS ON C(A)*A, A AND C SPD (PCGCA)
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
C***ROUTINES CALLED  MATVEC,MSSTOP,R1MACH,SCGCHK,SDOT,SNRM2
C***END PROLOGUE  SCRIND
C
C     TO SAVE VECTOR COPIES THE ITERATION IS UNROLLED
C
C     *** DECLARATIONS ***
CCCCCCIMPLICIT  DOUBLE PRECISION(A-H,O-Z)
      EXTERNAL  MATVEC
      DIMENSION IPARAM(*),RPARAM(*),X(N),B(N),R(N),AAP(N),AP(N)
      DIMENSION POLD(N),APOLD(N),D(*),E(*),CNDWK(*),IWORK(*)
C
C***FIRST EXECUTABLE STATEMENT  SCRIND
 1    CONTINUE
C
C     *** INITIALIZE INPUT PARAMETERS ***
      IOUNIT = IPARAM(5)
      ISTOP  = IPARAM(8)
      ITMAX  = IPARAM(9)
      ICYCLE = IPARAM(31)
      NCE    = IPARAM(32)
      KMAX   = ICYCLE*NCE
      ERRTOL = RPARAM(1)
      CONDA  = AMAX1(1.0E0, RPARAM(31))
      EPSLON = SQRT(R1MACH(4))
C
C     *** CHECK THE INPUT PARAMETERS ***
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,6)
 6    FORMAT(' THE METHOD IS CONJUGATE RESIDUALS (CRIND)', /)
      CALL SCGCHK(IPARAM,RPARAM,N)
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,8) CONDA
 8    FORMAT(4X, 'CONDA  = ', E12.5, /)
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,10)
 10   FORMAT(' RESID  = 2-NORM OF R', /,
     2       ' RELRSD = RESID / INITIAL RESID', /,
     3       ' COND(A) USED IN STOPPING CRITERION', /)
C
C     *** INITIALIZE D(1), EIGMIN, EIGMAX, ITERS ***
      D(1)   = 0.0E0
      EIGMIN = R1MACH(2)
      EIGMAX = R1MACH(1)
      ITERS  = 0
C
C     *** COMPUTE STOPPING CRITERION DENOMINATOR ***
      DENOM = 1.0E0
      IF (ISTOP .EQ. 0) DENOM = SNRM2(N,B,1)
      IF (ISTOP .EQ. 2) DENOM = SNRM2(N,B,1)
      IF (ISTOP .EQ. 4) DENOM = SNRM2(N,B,1)
C
C     *** TELL MSSTOP WHETHER OR NOT I AM SUPPLYING THE STOPPING QUANTITY ***
      IDO = 1
C
C     *** COMPUTE THE INITIAL RESIDUAL ***
      CALL MATVEC(0,A,IA,WDUMMY,X,R,N)
      DO 20 I = 1, N
         R(I) = B(I) - R(I)
 20   CONTINUE
      R0NORM = SNRM2(N,R,1)
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,25) R0NORM
 25   FORMAT(' INITIAL RESIDUAL = ', E12.5, /)
C
C     *** CHECK THE INITIAL RESIDUAL ***
      JSTOP = MSSTOP(ISTOP,ITERS,ITMAX,ERRTOL,STPTST,IERROR,
     2              RDUMM,SDUMM,ZDUMM,N,R0NORM,R0NORM,R0NORM,
     3              DENOM,CONDA,IDO)
      IF (JSTOP .EQ. 1) GOTO 90
C
C     *** INITIALIZE P, POLD, AP, APOLD ***
      CALL MATVEC(0,A,IA,WDUMMY,R,AP,N)
      DENNEW = 1.0E0
      DO 26 I = 1, N
         B(I) = R(I)
         POLD(I) = 0.0
         APOLD(I) = 0.0
 26   CONTINUE
C
C     *** UPDATE ITERS ***
 30   ITERS = ITERS + 1
C
C     *** COMPUTE NEW X AND R ***
      DENOLD = DENNEW
      DENNEW = SNRM2(N,AP,1)**2
      ALFOLD = ALPHA
      ALPHA =  SDOT(N,R,1,AP,1) / DENNEW
      DO 32 I = 1, N
         X(I) = X(I) + ALPHA*B(I)
         R(I) = R(I) - ALPHA*AP(I)
 32   CONTINUE
C
C     *** CHECK NEW R ***
      RNORM = SNRM2(N,R,1)
      IF (IOUNIT .GT. 0) WRITE(IOUNIT,35) ITERS, RNORM, RNORM/R0NORM
 35   FORMAT(' ITERS = ',I5,4X, 'RESID = ',E12.5,4X, 'RELRSD = ',E12.5)
C
C     *** TEST TO HALT ***
      JSTOP = MSSTOP(ISTOP,ITERS,ITMAX,ERRTOL,STPTST,IERROR,
     2              RDUMM,SDUMM,ZDUMM,N,RNORM,RNORM,RNORM,
     3              DENOM,CONDA,IDO)
      IF (JSTOP .EQ. 1) GOTO 90
C
C     *** CASE I: ALPHA SMALL ***
      IF (ABS(ALPHA) .LE. EPSLON) THEN
C
C        *** DETERMINE COEF ***
         IF (ABS(ALFOLD) .LE. EPSLON) THEN
            COEF = 1.0E0
         ELSE
            COEF = -1.0E0/ALFOLD
         END IF
C
C        *** COMPUTE NEW P, AP ***
         CALL MATVEC(0,A,IA,WDUMMY,AP,AAP,N)
         SIGMA = COEF*DENNEW / DENOLD
         GAMMA = SDOT(N,AAP,1,AP,1)/DENNEW
         DO 44 I = 1, N
            TEMPP = GAMMA*B(I) + SIGMA*POLD(I)
            POLD(I) = B(I)
            B(I) = AP(I) - TEMPP
            TEMPP = GAMMA*AP(I) + SIGMA*APOLD(I)
            APOLD(I) = AP(I)
            AP(I) = AAP(I) - TEMPP
 44      CONTINUE
      ELSE
C
C     *** CASE II: ALPHA LARGE ***
C 
C        *** COMPUTE NEW P, AP ***
         CALL MATVEC(0,A,IA,WDUMMY,R,AAP,N)
         BETA = SDOT(N,AAP,1,AP,1) / DENNEW
         DO 55 I = 1, N
            POLD(I) = B(I)
            APOLD(I) = AP(I)
            B(I) = R(I) - BETA*B(I)
            AP(I) = AAP(I) - BETA*AP(I)
 55      CONTINUE
      END IF
C
C     *** RESUME CR ITERATION ***
      GOTO 30
C
C     *** FINISHED: PASS BACK ITERATION INFO ***
 90   IPARAM(10) = ITERS
      RPARAM(2)  = STPTST
      RPARAM(31) = CONDA
      RPARAM(34) = RNORM/R0NORM
C
      RETURN
      END