/* ///////////////////////////////////////////////////////////////////////////
/// APBS -- Adaptive Poisson-Boltzmann Solver
///
///  Nathan A. Baker (nbaker@wasabi.ucsd.edu)
///  Dept. of Chemistry and Biochemistry
///  Dept. of Mathematics, Scientific Computing Group
///  University of California, San Diego 
///
///  Additional contributing authors listed in the code documentation.
///
/// Copyright � 1999. The Regents of the University of California (Regents).
/// All Rights Reserved. 
/// 
/// Permission to use, copy, modify, and distribute this software and its
/// documentation for educational, research, and not-for-profit purposes,
/// without fee and without a signed licensing agreement, is hereby granted,
/// provided that the above copyright notice, this paragraph and the
/// following two paragraphs appear in all copies, modifications, and
/// distributions.
/// 
/// IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
/// SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
/// ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
/// REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
/// 
/// REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE.  THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
/// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".  REGENTS HAS NO OBLIGATION
/// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
/// MODIFICATIONS. 
//////////////////////////////////////////////////////////////////////////// 
/// rcsid="$Id$"
//////////////////////////////////////////////////////////////////////////// */

/* ///////////////////////////////////////////////////////////////////////////
// File:     vpbe.c
//
// Purpose:  Class Vpbe: methods. 
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */

#include "apbscfg.h"
#include "apbs/vpbe.h"

/* ///////////////////////////////////////////////////////////////////////////
// Class Vpbe: Private method declaration
/////////////////////////////////////////////////////////////////////////// */


/* ///////////////////////////////////////////////////////////////////////////
// Class Vpbe: Inlineable methods
/////////////////////////////////////////////////////////////////////////// */
#if !defined(VINLINE_VPBE)

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getValist
//
// Purpose:  Get a pointer to the Valist (atom list) object
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC Valist* Vpbe_getValist(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->alist;

}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getVgm
//
// Purpose:  Get a pointer to the Gem (grid manager) object
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC Vgm* Vpbe_getVgm(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->gm; 

}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getVacc
//
// Purpose:  Get a pointer to the Vacc accessibility object 
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC Vacc* Vpbe_getVacc(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->acc; 

}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getVcsm
//
// Purpose:  Get a pointer to the Vcsm (charge-simplex map) object
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC Vcsm* Vpbe_getVcsm(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->csm; 

}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getIonConc
//
// Purpose:  Get the ionic strength in M
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getIonConc(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->ionConc; 

}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getTemperature
//
// Purpose:  Get the temperature in K
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getTemperature(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->T; 

}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSoluteDiel
//
// Purpose:  Get the solute dielectric
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSoluteDiel(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->soluteDiel; 

}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSoluteCenter
//
// Purpose:  Get the center of the solute molecule
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double* Vpbe_getSoluteCenter(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->soluteCenter; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSolventDiel
//
// Purpose:  Get the solvent dielectric
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSolventDiel(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->solventDiel; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSolventRadius
//
// Purpose:  Get the solvent radius in angstroms
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSolventRadius(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->solventRadius; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getIonRadius
//
// Purpose:  Get the ion probe radius in angstroms 
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getIonRadius(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->ionRadius; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getXkappa
//
// Purpose:  Get the Debye-Huckel parameter in reciprocal angstroms
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getXkappa(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->xkappa; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getDeblen
//
// Purpose:  Get the Debye length in angstroms
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getDeblen(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->deblen; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getZkappa2
//
// Purpose:  Get the squared modified Debye-Huckel parameter
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getZkappa2(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->zkappa2; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getZmagic
//
// Purpose:  Get the delta function scaling factor
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getZmagic(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   VASSERT(thee->paramFlag);
   return thee->zmagic; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSoluteRadius
//
// Purpose:  Get the radius of the solute molecule
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSoluteRadius(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->soluteRadius; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSoluteMaxX
//
// Purpose:  Get the max distance of solute molecule's atoms from center of
//           solute mol in x-direction
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSoluteMaxX(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->soluteMaxX; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSoluteMaxY
//
// Purpose:  Get the max distance of solute molecule's atoms from center of
//           solute mol in Y-direction
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSoluteMaxY(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->soluteMaxY; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSoluteMaxZ
//
// Purpose:  Get the max distance of solute molecule's atoms from center of
//           solute mol in Z-direction
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSoluteMaxZ(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->soluteMaxZ; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSoluteCharge
//
// Purpose:  Get the charge of the solute molecule
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getSoluteCharge(Vpbe *thee) { 

   VASSERT(thee != VNULL);
   return thee->soluteCharge; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getAtomColor
//
// Purpose:  Get mesh color information from the atoms.  Returns -1 if the atom
//           hasn't been initialized yet.
//
// Note:     This is a friend function of Vcsm
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC int Vpbe_getAtomColor(Vpbe *thee, int iatom) {

    int natoms;

    VASSERT(thee != VNULL);

    natoms = Valist_getNumberAtoms(thee->alist);
    VASSERT(iatom < natoms);

    return thee->csm->colors[iatom];
}
#endif /* if !defined(VINLINE_VPBE) */

/* ///////////////////////////////////////////////////////////////////////////
// Class Vpbe: Non-inlineable methods
/////////////////////////////////////////////////////////////////////////// */

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_ctor
//
// Purpose:  Construct the charge-vertex map, assign atoms to vertices,
//           and assign vertices to atoms
//
// Args:     alist    -- molecule for this Vpbe object
//           gm       -- the grid manager (when using MC, may be VNULL 
//                       otherwise)
//           methFlag -- method of solution associated with this Vpbe object
//                       = 0  --> use MC (adaptive multilevel FEM)
//                       = 1  --> use PMGC (multigrid)
//
// Notes:    The initial mesh must be sufficiently coarse for the
//           assignment procedures to be efficient.  
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC Vpbe* Vpbe_ctor(Valist *alist, Gem *gm, int methFlag) {

    /* Set up the structure */
    Vpbe *thee = VNULL;
    thee = Vmem_malloc(VNULL, 1, sizeof(Vpbe) );
    VASSERT( thee != VNULL);
    VASSERT( Vpbe_ctor2(thee, alist, gm, methFlag));

    return thee;
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_ctor2
//
// Purpose:  Construct the Vpbe object
//
// Notes:    Constructor broken into two parts for FORTRAN users.
//
// Returns:  1 if sucessful, 0 otherwise
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC int Vpbe_ctor2(Vpbe *thee, Valist *alist, Gem *gm, int methFlag) { 

    int iatom;
    double atomRadius;
    Vatom *atom;
    double center[3] = {0.0, 0.0, 0.0};
    double disp[3], dist, radius, charge, maxX, maxY, maxZ;

    /* Set up memory management object */
    thee->vmem = Vmem_ctor("APBS::VPBE");

    /* Set methFlag */
    thee->methFlag = methFlag;

    VASSERT(thee != VNULL);
    if (alist == VNULL) {
        Vnm_print(1,"Vpbe_ctor2: Got null pointer to Valist object!\n");
        return 0;
    }
    if ((gm == VNULL) && (thee->methFlag == 0)) {
        Vnm_print(1,"Vpbe_ctor2: Got null pointer to Gem object!\n");
        return 0;
    }

    /* **** STUFF THAT GETS DONE FOR EVERYONE **** */
    /* Set pointers */
    thee->alist = alist;
    thee->paramFlag = 0;

    /* Determine solute center */
    for (iatom=0; iatom<Valist_getNumberAtoms(thee->alist); iatom++) {
        center[0] += Vatom_getPosition(Valist_getAtom(thee->alist, iatom))[0];
        center[1] += Vatom_getPosition(Valist_getAtom(thee->alist, iatom))[1];
        center[2] += Vatom_getPosition(Valist_getAtom(thee->alist, iatom))[2];
    }
    center[0] = center[0]/((double)(Valist_getNumberAtoms(thee->alist)));
    center[1] = center[1]/((double)(Valist_getNumberAtoms(thee->alist)));
    center[2] = center[2]/((double)(Valist_getNumberAtoms(thee->alist)));
    thee->soluteCenter[0] = center[0];
    thee->soluteCenter[1] = center[1];
    thee->soluteCenter[2] = center[2];

    /* Determine solute radius and charge*/
    radius = 0;
    maxX = 0;
    maxY = 0;
    maxZ = 0;
    charge = 0;
    for (iatom=0; iatom<Valist_getNumberAtoms(thee->alist); iatom++) {
        atom = Valist_getAtom(thee->alist, iatom);
        disp[0] = (Vatom_getPosition(atom)[0] - center[0]);
        disp[1] = (Vatom_getPosition(atom)[1] - center[1]);
        disp[2] = (Vatom_getPosition(atom)[2] - center[2]);
        atomRadius = Vatom_getRadius(atom);
        dist = (disp[0]*disp[0]) + (disp[1]*disp[1]) + (disp[2]*disp[2]); 
        dist = VSQRT(dist) + atomRadius;
        if (dist > radius) radius = dist;
        if ((VABS(disp[0]) + atomRadius) > maxX) 
          maxX = (VABS(disp[0]) + atomRadius);
        if ((VABS(disp[1]) + atomRadius) > maxY) 
          maxY = (VABS(disp[1]) + atomRadius);
        if ((VABS(disp[2]) + atomRadius) > maxZ) 
          maxZ = (VABS(disp[2]) + atomRadius);
        charge += Vatom_getCharge(Valist_getAtom(thee->alist, iatom));
    }
    thee->soluteRadius = radius;
    thee->soluteMaxX = maxX;
    thee->soluteMaxY = maxY;
    thee->soluteMaxZ = maxZ;
    thee->soluteCharge = charge;

    /* **** METHOD-SPECIFIC STUFF **** */
    if (thee->methFlag == 0) {
        thee->gm = gm;
        /* Set up charge-simplex map */
        VASSERT((thee->csm = Vcsm_ctor(thee->alist, thee->gm)) != VNULL);
    } else if (thee->methFlag == 1) {

#if !defined(HAVE_PMGC_H) 
            Vnm_print(2, "Vpbe_ctor2: Not compiled with PMGC support!\n");
            return 0;
#endif

        thee->gm = VNULL;
    } else {
        Vnm_print(2, "Vpbe_ctor2: Invalid methFlag (=%d)!\n", methFlag);
        return 0;
    }

    return 1; 
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_dtor
//
// Purpose:  Destroy the charge-simplex map.
// 
// Notes:    Since the grid manager and atom list were allocated outside of
//           the Vpbe routines, they are not destroyed.
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC void Vpbe_dtor(Vpbe **thee) {
    if ((*thee) != VNULL) {
        Vpbe_dtor2(*thee);
        Vmem_free(VNULL, 1, sizeof(Vpbe), (void **)thee);
        (*thee) = VNULL;
    }
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_dtor2
//
// Purpose:  Destroy the atom object
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC void Vpbe_dtor2(Vpbe *thee) { 
    Vacc_dtor(&(thee->acc));
    Vmem_dtor(&(thee->vmem));
    if (thee->methFlag == 0) Vcsm_dtor(&(thee->csm));
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_initialize
//
// Purpose:  Set up parameters, Vacc objects, and charge-simplex map
//
// Arguments: ionConc       = ionic strength in M
//            ionRadius     = ionic probe radius in A
//            T             = temperature in K
//            soluteDiel    = solute dielectric (unitless)
//            solventDiel   = solvent dielectric (unitless)
//            solventRadius = solvent radius in Angstroms
//
// Notes:  Here's the original function coments from Mike
// notes:
// ------
//
//    kappa is defined as follows:
//
//       kappa^2 = (8 pi N_A e_c^2) I_s / (1000 eps_w k_B T)
//
//    note that the units here are:  esu^2 / erg-mole.
//    to obtain angstroms^{-2}, we multiply by 10^{-16}.
//
//    thus, in angstroms^{-2}, where k_B and e_c are in gaussian
//    rather than mks units, the proper value for kappa is:
//
//       kappa^2 = 8 pi N_A e_c^2 I_s / (1000 eps_w k_b T)  * 1.0e-16
//
//    where:
//
//       e_c = 4.803242384e-10 esu  (rather than 1.6021892e-19 coulombs)
//       k_B = 1.380662000e-16 ers  (rather than 1.380662000e-23 joules)
//
//    and the factor of 1.0e-16 results from converting cm^2 to anstroms^2,
//    noting that the 1000 in the denominator has converted m^3 to cm^3,
//    since the ionic strength I_s is assumed to have been provided in
//    moles
//    per liter, which is moles per 1000 cm^3.
//
//    some reference numbers in delphi:
//
//       salt = 1.0, deblen * I_s^{1/2} = 3.047000 angstroms
//       salt = 0.1, deblen * I_s^{1/2} = 9.635460 angstroms
//
//    delphi:  (kappa=0.328191663 * I_s^{1/2}, must have computed with
//    T!=298)
//    -------
//
//       deblen        = 1 / kappa
//                     = 1 / (0.328191663 * I_s^{1/2})
//                     = 3.047 / I_s^{1/2}    angstroms
//
//       kappa         = 1 / deblen
//                     = 0.328191663 * I_s^{1/2}    angstroms^{-1}
//
//       \bar{kappa}^2 = eps_w * kappa^2    angstroms^{-2}
//
//       debfact       = \bar{kappa}^2 * h^2
//                     = eps_w / (deblen * scale)**2
//
//    mike:  (kappa=0.325567 * I_s^{1/2}, with T=298)
//    -----
//
//       deblen        = 1 / kappa
//                     = 1 / (0.325567 * I_s^{1/2})
//                     = 3.071564378 * I_s^{1/2}   angstroms
//
//       kappa         = 1 / deblen
//                     = 0.325567 * I_s^{1/2}   angstroms^{-1}
//
//       \bar{kappa}^2 = eps_w * kappa^2   angstroms^{-2}
//
//       zkappa2       = \bar{kappa}^2  angstroms^{-2}
//
//    notes on scaling for the charges:
//    ---------------------------------
//
//       delphi:  (the 7049.484 seems to correspond to T=297.875)
//       -------
//
//          zmagic  = (4 * pi * e_c^2) * scale / (k_B T)
//                     / 6  (for diag scale of laplacean)
//                  = (4 * pi * e_c^2) / (6 h k_B T)
//                  = 7049.484 / (6 h)
//                  = 1174.914 / h
//
//       mike:    (the 7046.528838 corresponds to T=298)
//       -----
//
//          zmagic  = (4 * pi * e_c^2) / (k_B T)   (we scale the diagonal
//          later)
//                  = 7046.528838
//
//       since the units are esu^2 / erg, when converting to
//       angstroms^{-2},
//       we multiply by 1.0e8, yielding the 7046.528838 above.
//
// Author:   Nathan Baker and Michael Holst
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC void Vpbe_initialize(Vpbe *thee, double ionConc, double ionRadius,
                    double T, double soluteDiel, double solventDiel, 
                    double solventRadius) {

    const double N_A = 6.022045000e+23;
    const double e_c = 4.803242384e-10;
    const double k_B = 1.380662000e-16;
    const double pi  = 4. * VATAN(1.);

    double radius;
    double nhash;
 
    /* Set parameters */
    thee->ionConc = ionConc;
    thee->ionRadius = ionRadius;
    thee->T = T;
    thee->soluteDiel = soluteDiel;
    thee->solventDiel = solventDiel;
    thee->solventRadius = solventRadius;

    /* Compute parameters: 
     *
     * kappa^2 = (8 pi N_A e_c^2) I_s / (1000 eps_w k_B T)
     * kappa   = 0.325567 * I_s^{1/2}   angstroms^{-1}
     * deblen  = 1 / kappa
     *         = 3.071564378 * I_s^{1/2}   angstroms
     * \bar{kappa}^2 = eps_w * kappa^2 
     * zmagic  = (4 * pi * e_c^2) / (k_B T)   (we scale the diagonal later)
     *         = 7046.528838
     */
    if ((thee->T == 0.) || (thee->ionConc == 0.)) {
        thee->xkappa  = 0.;
        thee->deblen  = 0.;
        thee->zkappa2 = 0.;
    } else {
        thee->xkappa  = VSQRT( thee->ionConc * 1.0e-16 *
            ((8.0 * pi * N_A * e_c*e_c) / 
            (1000.0 * thee->solventDiel * k_B * T))
        );
        thee->deblen  = 1. / thee->xkappa;
        thee->zkappa2 = thee->solventDiel * VPOW(thee->xkappa,2.);
    }
    thee->zmagic  = ((4.0 * pi * e_c*e_c) / (k_B * thee->T)) * 1.0e+8;

    /* Compute accessibility objects */
    if (thee->ionRadius > thee->solventRadius) radius = thee->ionRadius;
    else radius = thee->solventRadius;
    nhash = 2.0*VPOW((double)Valist_getNumberAtoms(thee->alist), 1.0/3.0);
    thee->acc = Vacc_ctor(thee->alist, radius, (int)(nhash), (int)(nhash),
      (int)(nhash), 200);
    VASSERT(thee->acc != VNULL);

    /* MC-specific stuff */
    if (thee->methFlag == 0) {
        /* Compute charge-simplex map */
        Vcsm_init(thee->csm);
    }

    thee->paramFlag = 1;
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getSolution
//
// Purpose:  Get the electrostatic potential (in units of kT/e) at the
//           finest level of the passed AM object as a (newly allocated) array
//           of doubles and store the length in *length.  You'd better destroy
//           the returned array later!
//
// Notes:    Only meaningful for MC invocations of Vpbe (returns VNULL
//           otherwise)
//
// Author:   Nathan Baker and Michael Holst
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double* Vpbe_getSolution(Vpbe *thee, AM *am, int *length) { 

   int level, i;
   double *solution;
   double *theAnswer;

   VASSERT(thee != VNULL);
   if (thee->methFlag != 0) return VNULL;

   VASSERT(am != VNULL);
   VASSERT(thee->gm != VNULL);


   /* Get the max level from AM */
   level = AM_maxLevel(am);

   /* Copy the solution into the w0 vector */
   Bvec_copy(AM_alg(am, level)->W[W_w0], 
     AM_alg(am, level)->W[W_u]);
   /* Add the Dirichlet conditions */
   Bvec_axpy(AM_alg(am, level)->W[W_w0], 
     AM_alg(am, level)->W[W_ud], 1.);
   /* Get the data from the Bvec */
   solution = Bvec_data(AM_alg(am, level)->W[W_w0], 0);
   /* Get the length of the data from the Bvec */
   *length = Bvec_numRT(AM_alg(am, level)->W[W_w0]);
   /* Make sure that we got scalar data (only one block) for the solution
    * to the PBE */
   VASSERT(1 == Bvec_numB(AM_alg(am, level)->W[W_w0]));
   /* Allocate space for the returned vector and copy the solution into it */
   theAnswer = VNULL;
   theAnswer = Vmem_malloc(VNULL, *length, sizeof(double));
   VASSERT(theAnswer != VNULL);
   for (i=0; i<*length; i++) theAnswer[i] = solution[i];
   
   return theAnswer;
}


/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getLinearEnergy1
//
// Purpose:  using the solution at the finest mesh level, get the 
//           electrostatic energy using the free energy functional for the 
//           linearized Poisson-Boltzmann equation without removing any 
//           self-interaction terms (i.e., removing the reference state of
//           isolated charges present in an infinite dielectric continuum with 
//           the same relative permittivity as the interior of the protein).
//           In other words, we calculate
//             \[ G = \frac{1}{2} \sum_i q_i u(r_i) \]
//           and return the result in units of $k_B T$.  The argument color
//           allows the user to control the partition on which this energy
//           is calculated; if (color == -1) no restrictions are used.
//           The solution is obtained from the finest level of the passed AM
//           object, but atomic data from the Vpbe object is used to
//           calculate the energy
//
// Notes:    For MC implementations, the variable "system" should be a pointer 
//           to the AM object for the system.  
//           For PMGC implementations, the variable "system" should be a pointer
//           to the MGmlsys object for the system.
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getLinearEnergy1(Vpbe *thee, void *system, int color) { 

   double *sol; int nsol;
   double charge;
   double phi[4], phix[4][3], *position;
   int iatom, natoms;
   int isimp, nsimps;
   int icolor;
   int ivert;
   SS *simp;
   double energy = 0.0;
   double uval;

   AM *am;
#if defined(HAVE_PMGC_H)
   MGmlsys *mlsys;
   int I0, I1, J0, J1, K0, K1, nx, ny, nz, ihi, ilo, jhi, jlo, khi, klo;
   double xmax, xmin, ymax, ymin, zmax, zmin, hx, hy, hzed, ifloat, jfloat;
   double kfloat, dx, dy, dz;
#endif

   VASSERT(thee != VNULL);

   if (thee->methFlag == 0) {

       am = (AM *)system;
       VASSERT(am != VNULL);
       VASSERT(thee->gm != VNULL);
       VASSERT(thee->alist != VNULL);
       VASSERT(thee->csm != VNULL);
    
       /* Get the finest level solution */
       sol= VNULL;
       sol = Vpbe_getSolution(thee, am, &nsol);
       VASSERT(sol != VNULL);
    
    
       /* Make sure the number of entries in the solution array matches the
        * number of vertices currently in the mesh */
       if (nsol != Gem_numVV(thee->gm)) {
          Vnm_print(2, "Vpbe_getLinearEnergy1: Number of unknowns in solution does not match\n");
          Vnm_print(2, "Vpbe_getLinearEnergy1: number of vertices in mesh!!!  Bailing out!\n");
          VASSERT(0);
       }
    
       /* Now we do the sum over atoms... */
       natoms = Valist_getNumberAtoms(thee->alist);
       for (iatom=0; iatom<natoms; iatom++) {
           /* Get atom information */
           icolor = Vpbe_getAtomColor(thee, iatom);
           charge = Vatom_getCharge(Valist_getAtom(thee->alist, iatom));
           position = Vatom_getPosition(Valist_getAtom(thee->alist, iatom));
           /* Check if this atom belongs to the specified partition */
           if ((color>=0) && (icolor<0)) {
               Vnm_print(2, "Vpbe_getLinearEnergy1: Atom colors not set!\n");
               VASSERT(0);
           }
           if ((icolor==color) || (color<0)) { 
               /* Loop over the simps associated with this atom */
               nsimps =  Vcsm_getNumberSimplices(thee->csm, iatom);
               /* Get the first simp of the correct color; we can use just one
                * simplex for energy evaluations, but not for force 
                * evaluations */
               for (isimp=0; isimp<nsimps; isimp++) {
                   simp = Vcsm_getSimplex(thee->csm, isimp, iatom);
                   /* If we've asked for a particular partition AND if the atom 
                    * is our partition, then compute the energy */
                   if ((SS_chart(simp)==color)||(color<0)) {
                       /* Get the value of each basis function evaluated at this
                        * point */
                       Gem_pointInSimplexVal(thee->gm, simp, position, phi, phix);
                       for (ivert=0; ivert<SS_dimVV(simp); ivert++) {
                           uval = sol[VV_id(SS_vertex(simp,ivert))];
                           energy += (charge*phi[ivert]*uval);
                       } /* end for ivert */
                       /* We only use one simplex of the appropriate color for
                        * energy calculations, so break here */
                       break;
                   } /* endif (color) */
               } /* end for isimp */
           } 
       } /* end for iatom */
    
       /* Destroy the finest level solution */
       Vmem_free(VNULL, nsol, sizeof(double), (void **)&sol);
    
       /* Return the energy */
       return 0.5*energy;

    } else if (thee->methFlag == 1) {
#if defined(HAVE_PMGC_H)
        mlsys = (MGmlsys *)system;
        VASSERT(mlsys != VNULL);

        /* Get the fine level solution */
        sol = MGarray_d(mlsys->s[0]->u);

        energy = 0.0;
        natoms = Valist_getNumberAtoms(thee->alist);
        for (iatom=0; iatom<natoms; iatom++) {
            /* Get atom information */
            charge = Vatom_getCharge(Valist_getAtom(thee->alist, iatom));
            position = Vatom_getPosition(Valist_getAtom(thee->alist, iatom));

            /* Get mesh information */
            I0 = MGlsys_I0g(mlsys->s[0]);
            I1 = MGlsys_I1g(mlsys->s[0]);
            J0 = MGlsys_J0g(mlsys->s[0]);
            J1 = MGlsys_J1g(mlsys->s[0]);
            K0 = MGlsys_K0g(mlsys->s[0]);
            K1 = MGlsys_K1g(mlsys->s[0]);
            nx = MGlsys_nxg(mlsys->s[0]);
            ny = MGlsys_nyg(mlsys->s[0]);
            nz = MGlsys_nzg(mlsys->s[0]);
            hx = VABS(mlsys->s[0]->xc[1] - mlsys->s[0]->xc[0]);
            hy = VABS(mlsys->s[0]->yc[1] - mlsys->s[0]->yc[0]);
            hzed = VABS(mlsys->s[0]->zc[1] - mlsys->s[0]->zc[0]);
            xmax = mlsys->s[0]->xc[nx-1];
            xmin = mlsys->s[0]->xc[0];
            ymax = mlsys->s[0]->yc[ny-1];
            ymin = mlsys->s[0]->yc[0];
            zmax = mlsys->s[0]->zc[nz-1];
            zmin = mlsys->s[0]->zc[0];

            /* Make sure we're on the grid */
            if ((position[0]<=xmin) || (position[0]>=xmax)  || \
              (position[1]<=ymin) || (position[1]>=ymax)  || \
              (position[2]<=zmin) || (position[2]>=zmax)) {
                Vnm_print(2, "MGpde_fillco:  Atom #%d at (%4.3f, %4.3f, %4.3f) is off the mesh (ignoring)!\n",
                iatom, position[0], position[1], position[2]);
            } else {
                /* Figure out which vertices we're next to */
                ifloat = (position[0] - xmin)/hx;
                jfloat = (position[1] - xmin)/hy;
                kfloat = (position[2] - xmin)/hzed;

                ihi = (int)ceil(ifloat);
                ilo = (int)floor(ifloat);
                jhi = (int)ceil(jfloat);
                jlo = (int)floor(jfloat);
                khi = (int)ceil(kfloat);
                klo = (int)floor(kfloat);

                /* Now get trilinear interpolation constants */
                dx = ifloat - (double)(ilo);
                dy = jfloat - (double)(jlo);
                dz = kfloat - (double)(klo);
                uval =  dx*dy*dz*sol[II(ihi,jhi,khi)]
                      + dx*(1.0-dy)*dz*sol[II(ihi,jlo,khi)]
                      + dx*dy*(1.0-dz)*sol[II(ihi,jhi,klo)]
                      + dx*(1.0-dy)*(1.0-dz)*sol[II(ihi,jlo,klo)]
                      + (1.0-dx)*dy*dz*sol[II(ilo,jhi,khi)]
                      + (1.0-dx)*(1.0-dy)*dz*sol[II(ilo,jlo,khi)]
                      + (1.0-dx)*dy*(1.0-dz)*sol[II(ilo,jhi,klo)]
                      + (1.0-dx)*(1.0-dy)*(1.0-dz)*sol[II(ilo,jlo,klo)];
                energy += (uval*charge);
            }
        }

        energy = 0.5*energy;

        return energy;
#else /* if defined(HAVE_PMGC_H) */
        Vnm_print(2, "Vpbe_getLinearEnergy1: Not compiled with PMGC!\n");
        return 0.0;
#endif
    } else {
        Vnm_print(2, "Vpbe_getLinearEnergy1: invalid solution method (methFlag = %d)\n", 
          thee->methFlag);
        return 0.0;
    }
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getEnergyNorm2
//
// Purpose:  Calculate the square of the energy norm, i.e.
//                 u^T A u
//           The argument color allows the user to control the partition on
//           which this energy is calculated; if (color == -1) no restrictions
//           are used.  The solution is obtained from the finest level of the
//           passed AM object, but atomic data from the Vpbe object is used to
//           calculate the energy
//           
// Notes:    Large portions of this routine are borrowed from Mike Holst's
//           assem.c routines in MC.  THIS FUNCTION DOES NOT WORK FOR ANY
//           METHOD RIGHT NOW.
//
// Notes:    Currently only meaningful for MC invocations of Vpbe (returns
//           0.0 otherwise)
//           
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getEnergyNorm2(Vpbe *thee, Alg *alg, int color) {

    Bmat *A;
    Bvec *u, *Au;
    double norm2;

    if (thee->methFlag != 0) {
       Vnm_print(2, "Vpbe_getEnergyNorm2: Not implemented for methFlag %d\n",
         thee->methFlag);
       return 0.0;
    }

    /* Solution + Dirichlet conditions */
    Bvec_copy(alg->W[W_w0], alg->W[W_u]);
    u = alg->W[W_w0];
    Bvec_axpy(u, alg->W[W_ud], 1.);
    /* Stiffness matrix */
    A = alg->A;
    /* Work space */
    Au = alg->W[W_w1];

    if (color>=0) Vnm_print(2,"Vpbe_getEnergyNorm: color argument ignored!\n");

    /* Au = A u */
    Bvec_matvec(Au, u, A, 0);
    /* Calculate (u,Au) */
    norm2 = Bvec_dot(u,Au);

    return norm2;
}
    


/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getLinearEnergy2
//
// Purpose:  Calculate the energy from the energy norm, i.e. 
//                 G = (u, A u)/(8 pi)
//           for the linearized Poisson-Boltzmann equation without removing any
//           self-interaction terms (i.e., removing the reference state of
//           isolated charges present in an infinite dielectric continuum with
//           the same relative permittivity as the interior of the protein).
//           Return the result in units of $k_B T$.  The argument color allows
//           the user to control the partition on which this energy is
//           calculated; if (color == -1) no restrictions are used.  The
//           solution is obtained from the finest level of the passed AM
//           object, but atomic data from the Vpbe object is used to calculate
//           the energy.
//
// Notes:    Large portions of this routine are borrowed from Mike Holst's
//           assem.c routines in MC.
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getLinearEnergy2(Vpbe *thee, AM *am, int color) {

    double energy = 0.0;
    Alg *alg;    

    if (thee->methFlag != 0) {
       Vnm_print(2, "Vpbe_getLinearEnergy2: Not implemented for methFlag %d\n",
         thee->methFlag);
       return 0.0;
    }

    Vnm_print(2, "Vpbe_getLinearEnergy2: WARNING! This routine may be broken!\n");
    /* Get the algebra object for the finest level */
    alg = AM_alg(am, AM_maxLevel(am));

    /* Calculate the energy norm */
    energy = Vpbe_getEnergyNorm2(thee, alg, color);

    energy = energy/Vunit_pi/Vunit_pi/16.0;
    energy = energy*Vunit_eps0*10e-10;
    energy = energy/Vunit_ec/Vunit_ec*(Vunit_kb*thee->T);

    return energy;
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_getCoulombEnergy1
//
// Purpose:  Perform an inefficient double sum to calculate the Coulombic
//           energy of a set of charges in a homogeneous dielectric (with
//           permittivity equal to the protein interior) and zero ionic
//           strength.  Result is returned in units of k_B T.  The sum can be
//           restriction to charges present in simplices of specified color
//           (pcolor); if (color == -1) no restrictions are used.
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC double Vpbe_getCoulombEnergy1(Vpbe *thee) {

    int i, j, k, natoms;

    double dist, *ipos, *jpos, icharge, jcharge;
    double energy = 0.0;
    double eps, T;
    Vatom *iatom, *jatom;
    Valist *alist;
 
    VASSERT(thee != VNULL);
    alist = Vpbe_getValist(thee);
    VASSERT(alist != VNULL);
    natoms = Valist_getNumberAtoms(alist);
  
    /* Do the sum */ 
    for (i=0; i<natoms; i++) {
        iatom = Valist_getAtom(alist,i);
        icharge = Vatom_getCharge(iatom);
        ipos = Vatom_getPosition(iatom);
        for (j=i+1; j<natoms; j++) {
            jatom = Valist_getAtom(alist,j);
            jcharge = Vatom_getCharge(jatom);
            jpos = Vatom_getPosition(jatom);
            dist = 0;
            for (k=0; k<3; k++) dist += ((ipos[k]-jpos[k])*(ipos[k]-jpos[k]));
            dist = VSQRT(dist);
            energy = energy + icharge*jcharge/dist;
        }
    }

    /* Convert the result to J */
    T = Vpbe_getTemperature(thee);
    eps = Vpbe_getSoluteDiel(thee);
    energy = energy*Vunit_ec*Vunit_ec/(4*Vunit_pi*Vunit_eps0*eps*(1.0e-10));
   
    /* Scale by Boltzmann energy */
    energy = energy/(Vunit_kb*T);

    return energy;
}

/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_setAtomColors
//
// Purpose:  Transfer color information from partitioned mesh to the atoms.
//           In the case that a charge is shared between two partitions, the
//           partition color of the first simplex is selected.  Due to the
//           arbitrary nature of this selection, THIS METHOD SHOULD ONLY BE
//           USED IMMEDIATELY AFTER PARTITIONING!!!
//
// Note:     This is a friend function of Vcsm
// Note:     This has no meaning for thee->methFlag != 0
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC void Vpbe_setAtomColors(Vpbe *thee) {

#define VMAXLOCALCOLORSDONTREUSETHISVARIABLE 1024
    SS *simp;
    int i, natoms;

    VASSERT(thee != VNULL);

    if (thee->methFlag != 0) {
        Vnm_print(2, "Vpbe_setAtomColors: ignoring call for methFlag = %d\n",
          thee->methFlag);
        return;
    }

    natoms = Valist_getNumberAtoms(thee->alist);
    for (i=0; i<natoms; i++) {
        simp = Vcsm_getSimplex(thee->csm, 0, i);
        thee->csm->colors[i] = SS_chart(simp);
    }

}



/* ///////////////////////////////////////////////////////////////////////////
// Routine:  Vpbe_memChk
//
// Purpose:  Returns the bytes used by the specified object
//
// Author:   Nathan Baker
/////////////////////////////////////////////////////////////////////////// */
VPUBLIC int Vpbe_memChk(Vpbe *thee) {
   
    int memUse = 0;

    if (thee == VNULL) return 0;

    memUse = memUse + sizeof(Vpbe);
    if (thee->methFlag == 0) memUse = memUse + Vcsm_memChk(thee->csm);
    memUse = memUse + Vacc_memChk(thee->acc);

    return memUse;
}
