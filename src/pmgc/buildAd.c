#include "apbs/buildAd.h"
#include "apbs/vhal.h"

#include "apbs/vmatrix.h"

VPUBLIC void VbuildA(int* nx, int* ny, int* nz,
		int* ipkey, int* mgdisc, int* numdia,
		int* ipc, double* rpc,
		double* ac, double* cc, double* fc,
		double* xf, double* yf, double* zf,
		double* gxcf, double* gycf, double* gzcf,
		double* a1cf, double* a2cf, double* a3cf,
		double* ccf, double* fcf) {

    MAT2(ac, *nx * *ny * *nz, 14);

    ANNOUNCE_FUNCTION;

    if (*mgdisc == 0) {

        VbuildA_fv(nx, ny, nz,
                ipkey, numdia,
                ipc, rpc,
                RAT2(ac, 1,1), cc, fc,
                RAT2(ac, 1,2), RAT2(ac, 1,3), RAT2(ac, 1,4),
                xf, yf, zf,
                gxcf, gycf, gzcf,
                a1cf, a2cf, a3cf,
                ccf, fcf);

    } else if (*mgdisc == 1) {

        VbuildA_fe(nx, ny, nz,
                ipkey, numdia,
                ipc, rpc,
                RAT2(ac, 1, 1), cc, fc,
                RAT2(ac, 1, 2), RAT2(ac, 1, 3), RAT2(ac, 1, 4), RAT2(ac, 1, 5), RAT2(ac, 1, 6),
                RAT2(ac, 1, 7), RAT2(ac, 1, 8), RAT2(ac, 1, 9), RAT2(ac, 1,10),
                RAT2(ac, 1,11), RAT2(ac, 1,12), RAT2(ac, 1,13), RAT2(ac, 1,14),
                xf, yf, zf,
                gxcf, gycf, gzcf,
                a1cf, a2cf, a3cf,
                ccf, fcf);

	} else {

		Vnm_print(2, "VbuildA:  Invalid discretization requested.\n");
        /// @todo: use an APBS/MALOC method to quit/fail
		exit(EXIT_FAILURE);

	}
}



VPUBLIC void VbuildA_fv(int *nx, int *ny, int *nz,
		int *ipkey, int *numdia,
		int *ipc, double *rpc,
		double *oC, double *cc, double *fc, double *oE, double *oN, double *uC,
		double *xf, double *yf, double *zf,
		double *gxcf, double *gycf, double *gzcf,
		double *a1cf, double *a2cf, double *a3cf,
		double *ccf,  double *fcf) {

	int i, j, k;            // @todo: Doc

	/** @note:  The following variables are temporaries that are not necessarily
	 *          all needed as explicitly different variables.  They do not
	 *          persist beyond the scope of this function.  This set could
	 *          probably be reduced to at most 3 temporary variables.  Well
	 *          placed comments would go a lot farther in making the semantics
	 *          of the code plain rather than producing a huge slew of seemingly
	 *          homogeneous temporaries named using unclear abbreviations
	 */

	// @todo: Doc
	int ike, jke, kke;

	// @todo: Doc
	int nxm1, nym1, nzm1;

	// @todo: Doc
	double hx, hy, hz;

    // @todo: Doc
	double hxm1, hym1, hzm1;

	double coef_fc;   // @todo: Doc

	double bc_cond_e; // @todo: Doc
	double bc_cond_w; // @todo: Doc
	double bc_cond_n; // @todo: Doc
	double bc_cond_s; // @todo: Doc
	double bc_cond_u; // @todo: Doc
	double bc_cond_d; // @todo: Doc
    double coef_oE;   // @todo: Doc
    double coef_oN;   // @todo: Doc
    double coef_uC;   // @todo: Doc
    double coef_oEm1; // @todo: Doc
    double coef_oNm1; // @todo: Doc
    double coef_uCm1; // @todo: Doc

    double diag;      // @todo: Doc

    MAT3(  fc, *nx, *ny, *nz);
    MAT3( fcf, *nx, *ny, *nz);
    MAT3(  cc, *nx, *ny, *nz);
    MAT3( ccf, *nx, *ny, *nz);
    MAT3(  oC, *nx, *ny, *nz);
    MAT3(a1cf, *nx, *ny, *nz);
    MAT3(a2cf, *nx, *ny, *nz);
    MAT3(a3cf, *nx, *ny, *nz);
    MAT3(  uC, *nx, *ny, *nz);
    MAT3(  oN, *nx, *ny, *nz);
    MAT3(  oE, *nx, *ny, *nz);
    MAT3(gxcf, *ny, *nz,   2);
    MAT3(gycf, *nx, *nz,   2);
    MAT3(gzcf, *nx, *ny,   2);

    ANNOUNCE_FUNCTION;

    // Save the problem key with this operator.  @todo:  What?
    VAT(ipc, 10) = *ipkey;

    // Note how many nonzeros in this discretization stencil
    VAT(ipc, 11) = 7;
    VAT(ipc, 12) = 1;
    *numdia = 4;

    // Define n and determine number of mesh points
    nxm1 = *nx - 1;
    nym1 = *ny - 1;
    nzm1 = *nz - 1;

    // Determine diag scale factor
    // (would like something close to ones on the main diagonal)
    // @todo: Make a more meaningful comment
    diag = 1.0;



    /* *********************************************************************
     * *** interior points ***
     * ********************************************************************* */

    // build the operator
    for(k=2; k<=*nz-1; k++) {

    	hzm1 = VAT(zf, k)   - VAT(zf, k-1);
    	hz   = VAT(zf, k+1) - VAT(zf, k);

    	for(j=2; j<=*ny-1; j++) {

    		hym1 = VAT(yf, j)   - VAT(yf, j-1);
    		hy   = VAT(yf, j+1) - VAT(yf, j);

    		for(i=2; i<=*nx-1; i++) {

    			hxm1 = VAT(xf, i)   - VAT(xf, i-1);
    			hx   = VAT(xf, i+1) - VAT(xf, i);

    			// Calculate some coefficients
    			/** @note that these and the running OC calculation could
    			 *        easily be pushed down into the step by step
    			 *        compuation of neighbors.  That would alleviate some
    			 *        of the temporary madness
    			 */

    			coef_oE   = diag * (hym1 + hy) * (hzm1 + hz) / (4.0 * hx);
    			coef_oEm1 = diag * (hym1 + hy) * (hzm1 + hz) / (4.0 * hxm1);
    			coef_oN   = diag * (hxm1 + hx) * (hzm1 + hz) / (4.0 * hy);
    			coef_oNm1 = diag * (hxm1 + hx) * (hzm1 + hz) / (4.0 * hym1);
    			coef_uC   = diag * (hxm1 + hx) * (hym1 + hy) / (4.0 * hz);
    			coef_uCm1 = diag * (hxm1 + hx) * (hym1 + hy) / (4.0 * hzm1);
    			coef_fc   = diag * (hxm1 + hx) * (hym1 + hy) * (hzm1 + hz) / 8.0;

                // Calculate the coefficient and source function
    			VAT3(fc, i, j, k) = coef_fc * VAT3(fcf, i, j, k);
    			VAT3(cc, i, j, k) = coef_fc * VAT3(ccf, i, j, k);

    			// Calculate the diagonal for matvecs and smoothings
			    VAT3(oC, i, j, k) = coef_oE   * VAT3(a1cf,   i,   j,   k) +
			    		      coef_oEm1 * VAT3(a1cf, i-1,   j,   k) +
			    		      coef_oN   * VAT3(a2cf,   i,   j,   k) +
			    		      coef_oNm1 * VAT3(a2cf,   i, j-1,   k) +
			    		      coef_uC   * VAT3(a3cf,   i,   j,   k) +
			    		      coef_uCm1 * VAT3(a3cf,   i,   j, k-1);

                // Calculate the east neighbor
			    ike = VMIN2(1, VABS(i - nxm1));
			    VAT3(oE, i, j, k) = ike * coef_oE * VAT3(a1cf, i, j, k);
			    bc_cond_e = (1 - ike) * coef_oE * VAT3(a1cf, i, j, k) * VAT3(gxcf,  j, k, 2);
			    VAT3(fc, i, j, k) += bc_cond_e;

			    // Calculate the north neighbor
			    jke = VMIN2(1, VABS(j - nym1));
			    VAT3(oN, i, j, k) = jke * coef_oN * VAT3(a2cf, i, j, k);
			    bc_cond_n = (1 - jke) * coef_oN * VAT3(a2cf, i, j, k) * VAT3(gycf, i, k, 2);
			    VAT3(fc, i, j, k) += bc_cond_n;

			    // Calculate the up neighbor
			    kke = VMIN2(1, VABS(k - nzm1));
			    VAT3(uC, i, j, k) = kke * coef_uC * VAT3(a3cf, i, j, k);
			    bc_cond_u = (1 - kke) * coef_uC * VAT3(a3cf, i, j, k) * VAT3(gzcf, i, j, 2);
			    VAT3(fc, i, j, k) += bc_cond_u;

			    // Calculate the west neighbor (just handle b.c.)
			    ike = VMIN2(1, VABS(i - 2));
			    bc_cond_w = (1 - ike) * coef_oEm1 * VAT3(a1cf, i-1, j, k) * VAT3(gxcf,  j, k, 1);
			    VAT3(fc, i, j, k) += bc_cond_w;

			    // Calculate the south neighbor (just handle b.c.)
			    jke = VMIN2(1, VABS(j - 2));
			    bc_cond_s = (1 - jke) * coef_oNm1 * VAT3(a2cf, i, j-1, k) * VAT3(gycf, i, k, 1);
			    VAT3(fc, i, j, k) += bc_cond_s;

			    // Calculate the down neighbor (just handle b.c.)
			    kke = VMIN2(1, VABS(k - 2));
			    bc_cond_d = (1 - kke) * coef_uCm1 * VAT3(a3cf, i, j, k-1) * VAT3(gzcf, i, j, 1);
			    VAT3(fc, i, j, k) += bc_cond_d;
    		}
    	}
    }
}



VPUBLIC void VbuildA_fe(int *nx, int *ny, int *nz,
		int *ipkey, int *numdia,
		int *ipc, double *rpc,
		double *oC, double *cc, double *fc,
                double *oE, double *oN, double *uC,
                double* oNE, double* oNW,
                double* uE, double* uW,     
                double* uN, double* uS,     
                double* uNE, double* uNW, double* uSE, double* uSW,
		double *xf, double *yf, double *zf,
		double *gxcf, double *gycf, double *gzcf,
		double *a1cf, double *a2cf, double *a3cf,
		double *ccf,  double *fcf) {
    VABORT_MSG0("Untranslated Component: from buildAd.f");
}