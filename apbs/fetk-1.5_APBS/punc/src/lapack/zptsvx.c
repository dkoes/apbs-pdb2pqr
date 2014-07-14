/* ./src_f77/zptsvx.f -- translated by f2c (version 20030320).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include <punc/vf2c.h>

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int zptsvx_(char *fact, integer *n, integer *nrhs, 
	doublereal *d__, doublecomplex *e, doublereal *df, doublecomplex *ef, 
	doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, 
	doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *
	work, doublereal *rwork, integer *info, ftnlen fact_len)
{
    /* System generated locals */
    integer b_dim1, b_offset, x_dim1, x_offset, i__1;

    /* Local variables */
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static doublereal anorm;
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *), zcopy_(integer *, doublecomplex *, 
	    integer *, doublecomplex *, integer *);
    extern doublereal dlamch_(char *, ftnlen);
    static logical nofact;
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    extern doublereal zlanht_(char *, integer *, doublereal *, doublecomplex *
	    , ftnlen);
    extern /* Subroutine */ int zlacpy_(char *, integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, ftnlen), 
	    zptcon_(integer *, doublereal *, doublecomplex *, doublereal *, 
	    doublereal *, doublereal *, integer *), zptrfs_(char *, integer *,
	     integer *, doublereal *, doublecomplex *, doublereal *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    integer *, doublereal *, doublereal *, doublecomplex *, 
	    doublereal *, integer *, ftnlen), zpttrf_(integer *, doublereal *,
	     doublecomplex *, integer *), zpttrs_(char *, integer *, integer *
	    , doublereal *, doublecomplex *, doublecomplex *, integer *, 
	    integer *, ftnlen);


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     June 30, 1999 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZPTSVX uses the factorization A = L*D*L**H to compute the solution */
/*  to a complex system of linear equations A*X = B, where A is an */
/*  N-by-N Hermitian positive definite tridiagonal matrix and X and B */
/*  are N-by-NRHS matrices. */

/*  Error bounds on the solution and a condition estimate are also */
/*  provided. */

/*  Description */
/*  =========== */

/*  The following steps are performed: */

/*  1. If FACT = 'N', the matrix A is factored as A = L*D*L**H, where L */
/*     is a unit lower bidiagonal matrix and D is diagonal.  The */
/*     factorization can also be regarded as having the form */
/*     A = U**H*D*U. */

/*  2. If the leading i-by-i principal minor is not positive definite, */
/*     then the routine returns with INFO = i. Otherwise, the factored */
/*     form of A is used to estimate the condition number of the matrix */
/*     A.  If the reciprocal of the condition number is less than machine */
/*     precision, INFO = N+1 is returned as a warning, but the routine */
/*     still goes on to solve for X and compute error bounds as */
/*     described below. */

/*  3. The system of equations is solved for X using the factored form */
/*     of A. */

/*  4. Iterative refinement is applied to improve the computed solution */
/*     matrix and calculate error bounds and backward error estimates */
/*     for it. */

/*  Arguments */
/*  ========= */

/*  FACT    (input) CHARACTER*1 */
/*          Specifies whether or not the factored form of the matrix */
/*          A is supplied on entry. */
/*          = 'F':  On entry, DF and EF contain the factored form of A. */
/*                  D, E, DF, and EF will not be modified. */
/*          = 'N':  The matrix A will be copied to DF and EF and */
/*                  factored. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  NRHS    (input) INTEGER */
/*          The number of right hand sides, i.e., the number of columns */
/*          of the matrices B and X.  NRHS >= 0. */

/*  D       (input) DOUBLE PRECISION array, dimension (N) */
/*          The n diagonal elements of the tridiagonal matrix A. */

/*  E       (input) COMPLEX*16 array, dimension (N-1) */
/*          The (n-1) subdiagonal elements of the tridiagonal matrix A. */

/*  DF      (input or output) DOUBLE PRECISION array, dimension (N) */
/*          If FACT = 'F', then DF is an input argument and on entry */
/*          contains the n diagonal elements of the diagonal matrix D */
/*          from the L*D*L**H factorization of A. */
/*          If FACT = 'N', then DF is an output argument and on exit */
/*          contains the n diagonal elements of the diagonal matrix D */
/*          from the L*D*L**H factorization of A. */

/*  EF      (input or output) COMPLEX*16 array, dimension (N-1) */
/*          If FACT = 'F', then EF is an input argument and on entry */
/*          contains the (n-1) subdiagonal elements of the unit */
/*          bidiagonal factor L from the L*D*L**H factorization of A. */
/*          If FACT = 'N', then EF is an output argument and on exit */
/*          contains the (n-1) subdiagonal elements of the unit */
/*          bidiagonal factor L from the L*D*L**H factorization of A. */

/*  B       (input) COMPLEX*16 array, dimension (LDB,NRHS) */
/*          The N-by-NRHS right hand side matrix B. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B.  LDB >= max(1,N). */

/*  X       (output) COMPLEX*16 array, dimension (LDX,NRHS) */
/*          If INFO = 0 or INFO = N+1, the N-by-NRHS solution matrix X. */

/*  LDX     (input) INTEGER */
/*          The leading dimension of the array X.  LDX >= max(1,N). */

/*  RCOND   (output) DOUBLE PRECISION */
/*          The reciprocal condition number of the matrix A.  If RCOND */
/*          is less than the machine precision (in particular, if */
/*          RCOND = 0), the matrix is singular to working precision. */
/*          This condition is indicated by a return code of INFO > 0. */

/*  FERR    (output) DOUBLE PRECISION array, dimension (NRHS) */
/*          The forward error bound for each solution vector */
/*          X(j) (the j-th column of the solution matrix X). */
/*          If XTRUE is the true solution corresponding to X(j), FERR(j) */
/*          is an estimated upper bound for the magnitude of the largest */
/*          element in (X(j) - XTRUE) divided by the magnitude of the */
/*          largest element in X(j). */

/*  BERR    (output) DOUBLE PRECISION array, dimension (NRHS) */
/*          The componentwise relative backward error of each solution */
/*          vector X(j) (i.e., the smallest relative change in any */
/*          element of A or B that makes X(j) an exact solution). */

/*  WORK    (workspace) COMPLEX*16 array, dimension (N) */

/*  RWORK   (workspace) DOUBLE PRECISION array, dimension (N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, and i is */
/*                <= N:  the leading minor of order i of A is */
/*                       not positive definite, so the factorization */
/*                       could not be completed, and the solution has not */
/*                       been computed. RCOND = 0 is returned. */
/*                = N+1: U is nonsingular, but RCOND is less than machine */
/*                       precision, meaning that the matrix is singular */
/*                       to working precision.  Nevertheless, the */
/*                       solution and error bounds are computed because */
/*                       there are a number of situations where the */
/*                       computed solution can be more accurate than the */
/*                       value of RCOND would suggest. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --d__;
    --e;
    --df;
    --ef;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --ferr;
    --berr;
    --work;
    --rwork;

    /* Function Body */
    *info = 0;
    nofact = lsame_(fact, "N", (ftnlen)1, (ftnlen)1);
    if (! nofact && ! lsame_(fact, "F", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*nrhs < 0) {
	*info = -3;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    } else if (*ldx < max(1,*n)) {
	*info = -11;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZPTSVX", &i__1, (ftnlen)6);
	return 0;
    }

    if (nofact) {

/*        Compute the L*D*L' (or U'*D*U) factorization of A. */

	dcopy_(n, &d__[1], &c__1, &df[1], &c__1);
	if (*n > 1) {
	    i__1 = *n - 1;
	    zcopy_(&i__1, &e[1], &c__1, &ef[1], &c__1);
	}
	zpttrf_(n, &df[1], &ef[1], info);

/*        Return if INFO is non-zero. */

	if (*info != 0) {
	    if (*info > 0) {
		*rcond = 0.;
	    }
	    return 0;
	}
    }

/*     Compute the norm of the matrix A. */

    anorm = zlanht_("1", n, &d__[1], &e[1], (ftnlen)1);

/*     Compute the reciprocal of the condition number of A. */

    zptcon_(n, &df[1], &ef[1], &anorm, rcond, &rwork[1], info);

/*     Set INFO = N+1 if the matrix is singular to working precision. */

    if (*rcond < dlamch_("Epsilon", (ftnlen)7)) {
	*info = *n + 1;
    }

/*     Compute the solution vectors X. */

    zlacpy_("Full", n, nrhs, &b[b_offset], ldb, &x[x_offset], ldx, (ftnlen)4);
    zpttrs_("Lower", n, nrhs, &df[1], &ef[1], &x[x_offset], ldx, info, (
	    ftnlen)5);

/*     Use iterative refinement to improve the computed solutions and */
/*     compute error bounds and backward error estimates for them. */

    zptrfs_("Lower", n, nrhs, &d__[1], &e[1], &df[1], &ef[1], &b[b_offset], 
	    ldb, &x[x_offset], ldx, &ferr[1], &berr[1], &work[1], &rwork[1], 
	    info, (ftnlen)5);

    return 0;

/*     End of ZPTSVX */

} /* zptsvx_ */

