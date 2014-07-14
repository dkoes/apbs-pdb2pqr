/* ./src_f77/cptsv.f -- translated by f2c (version 20030320).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include <punc/vf2c.h>

/* Subroutine */ int cptsv_(integer *n, integer *nrhs, real *d__, complex *e, 
	complex *b, integer *ldb, integer *info)
{
    /* System generated locals */
    integer b_dim1, b_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen), cpttrf_(
	    integer *, real *, complex *, integer *), cpttrs_(char *, integer 
	    *, integer *, real *, complex *, complex *, integer *, integer *, 
	    ftnlen);


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     February 25, 1997 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CPTSV computes the solution to a complex system of linear equations */
/*  A*X = B, where A is an N-by-N Hermitian positive definite tridiagonal */
/*  matrix, and X and B are N-by-NRHS matrices. */

/*  A is factored as A = L*D*L**H, and the factored form of A is then */
/*  used to solve the system of equations. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  NRHS    (input) INTEGER */
/*          The number of right hand sides, i.e., the number of columns */
/*          of the matrix B.  NRHS >= 0. */

/*  D       (input/output) REAL array, dimension (N) */
/*          On entry, the n diagonal elements of the tridiagonal matrix */
/*          A.  On exit, the n diagonal elements of the diagonal matrix */
/*          D from the factorization A = L*D*L**H. */

/*  E       (input/output) COMPLEX array, dimension (N-1) */
/*          On entry, the (n-1) subdiagonal elements of the tridiagonal */
/*          matrix A.  On exit, the (n-1) subdiagonal elements of the */
/*          unit bidiagonal factor L from the L*D*L**H factorization of */
/*          A.  E can also be regarded as the superdiagonal of the unit */
/*          bidiagonal factor U from the U**H*D*U factorization of A. */

/*  B       (input/output) COMPLEX array, dimension (LDB,NRHS) */
/*          On entry, the N-by-NRHS right hand side matrix B. */
/*          On exit, if INFO = 0, the N-by-NRHS solution matrix X. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B.  LDB >= max(1,N). */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, the leading minor of order i is not */
/*                positive definite, and the solution has not been */
/*                computed.  The factorization has not been completed */
/*                unless i = N. */

/*  ===================================================================== */

/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --d__;
    --e;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    *info = 0;
    if (*n < 0) {
	*info = -1;
    } else if (*nrhs < 0) {
	*info = -2;
    } else if (*ldb < max(1,*n)) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CPTSV ", &i__1, (ftnlen)6);
	return 0;
    }

/*     Compute the L*D*L' (or U'*D*U) factorization of A. */

    cpttrf_(n, &d__[1], &e[1], info);
    if (*info == 0) {

/*        Solve the system A*X = B, overwriting B with X. */

	cpttrs_("Lower", n, nrhs, &d__[1], &e[1], &b[b_offset], ldb, info, (
		ftnlen)5);
    }
    return 0;

/*     End of CPTSV */

} /* cptsv_ */

