/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2012 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the ZIB Academic License.         */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file   circle.c
 * @brief  Solving the Circle Enclosing Points Problem
 * @author Stefan Vigerske
 *
 * This example shows how to setup second-order-cone constraints in SCIP when using SCIP as callable library.
 * The example implements a model for the computation of a smallest circle that contains a number of given points
 * in the plane.
 *
 * The model is taken from the GAMS model library:
 * http://www.gams.com/modlib/libhtml/circle.htm
 *
 * See also: http://en.wikipedia.org/wiki/Smallest_circle_problem
 *
 * Given n points in the plane with coordinates \f$(x_i, y_i)\f$, the task is to find a coordinates \f$(a,b)\f$
 * and a minimal radius \f$r \geq 0\f$, such that \f$\sqrt{(x_i-a)^2 + (y_i-b)^2} \leq r\f$.
 * The latter are second-order-cone constraints.
 */

/*--+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#include <stdio.h>
#include <math.h>

#include "scip/scip.h"
#include "scip/scipdefplugins.h"

extern
SCIP_RETCODE runCircle(void);

/** number of points to enclose by a circle */
static const int npoints = 10;

/** seed for random number generator */
static const unsigned int randseed = 42.0;

/** sets up problem */
static
SCIP_RETCODE setupProblem(
   SCIP*                 scip                /**< SCIP data structure */
   )
{
   SCIP_VAR* a;
   SCIP_VAR* b;
   SCIP_VAR* r;

   unsigned int seed;
   char name[SCIP_MAXSTRLEN];
   int i;

   /* create empty problem */
   SCIP_CALL( SCIPcreateProbBasic(scip, "circle") );

   /* create variables and add to problem */
   SCIP_CALL( SCIPcreateVarBasic(scip, &a, "a", -SCIPinfinity(scip), SCIPinfinity(scip), 0.0, SCIP_VARTYPE_CONTINUOUS) );
   SCIP_CALL( SCIPcreateVarBasic(scip, &b, "b", -SCIPinfinity(scip), SCIPinfinity(scip), 0.0, SCIP_VARTYPE_CONTINUOUS) );
   SCIP_CALL( SCIPcreateVarBasic(scip, &r, "r", 0, SCIPinfinity(scip), 1.0, SCIP_VARTYPE_CONTINUOUS) );

   SCIP_CALL( SCIPaddVar(scip, a) );
   SCIP_CALL( SCIPaddVar(scip, b) );
   SCIP_CALL( SCIPaddVar(scip, r) );

   /* create soc constraints, add to problem, and forget about them */
   seed = randseed;
   for( i = 0; i < npoints; ++i )
   {
      SCIP_CONS* cons;
      SCIP_VAR* ab[2];
      SCIP_Real xy[2];

      ab[0] = a;
      ab[1] = b;
      xy[0] = -SCIPgetRandomReal(1.0, 10.0, &seed);
      xy[1] = -SCIPgetRandomReal(1.0, 10.0, &seed);

      (void) SCIPsnprintf(name, SCIP_MAXSTRLEN, "point%d", i);
      SCIP_CALL( SCIPcreateConsBasicSOC(scip, &cons, name, 2, ab, NULL, xy, 0.0, r, 1.0, 0.0) );
      SCIP_CALL( SCIPaddCons(scip, cons) );
      SCIP_CALL( SCIPreleaseCons(scip, &cons) );
   }

   /* release variables */
   SCIP_CALL( SCIPreleaseVar(scip, &a) );
   SCIP_CALL( SCIPreleaseVar(scip, &b) );
   SCIP_CALL( SCIPreleaseVar(scip, &r) );

   return SCIP_OKAY;
}

/* runs circle enclosing example */
SCIP_RETCODE runCircle(void)
{
   SCIP* scip;

   SCIP_CALL( SCIPcreate(&scip) );
   SCIP_CALL( SCIPincludeDefaultPlugins(scip) );

   SCIPinfoMessage(scip, NULL, "\n");
   SCIPinfoMessage(scip, NULL, "*********************************************\n");
   SCIPinfoMessage(scip, NULL, "* Running Smallest Enclosing Circle Problem *\n");
   SCIPinfoMessage(scip, NULL, "*********************************************\n");
   SCIPinfoMessage(scip, NULL, "\n");

   SCIP_CALL( setupProblem(scip) );

   SCIPinfoMessage(scip, NULL, "Original problem:\n");
   SCIP_CALL( SCIPprintOrigProblem(scip, NULL, "cip", FALSE) );

   SCIPinfoMessage(scip, NULL, "\nSolving...\n");
   SCIP_CALL( SCIPsolve(scip) );

   SCIP_CALL( SCIPfreeTransform(scip) );

   if( SCIPgetNSols(scip) > 0 )
   {
      SCIPinfoMessage(scip, NULL, "\nSolution:\n");
      SCIP_CALL( SCIPprintSol(scip, SCIPgetBestSol(scip), NULL, FALSE) );
   }

   SCIP_CALL( SCIPfree(&scip) );

   return SCIP_OKAY;
}
