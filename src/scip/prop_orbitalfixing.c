/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2017 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the ZIB Academic License.         */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file   prop_orbitalfixing.c
 * @brief  propagator for orbital fixing
 * @author Marc Pfetsch
 *
 * @todo Turn off propagator in subtrees.
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#include "prop_orbitalfixing.h"

#include <scip/pub_tree.h>
#include <scip/pub_table.h>

#include "presol_symmetry.h"
#include "presol_symbreak.h"

/* propagator properties */
#define PROP_NAME              "orbitalfixing"
#define PROP_DESC              "propagator for orbital fixing"
#define PROP_TIMING    SCIP_PROPTIMING_BEFORELP   /**< propagation timing mask */
#define PROP_PRIORITY          -1000000           /**< propagator priority */
#define PROP_FREQ                    -1           /**< propagator frequency */
#define PROP_DELAY                FALSE           /**< should propagation method be delayed, if other propagators found reductions? */

/* output table properties */
#define TABLE_NAME_ORBITALFIXING        "orbitalfixing"
#define TABLE_DESC_ORBITALFIXING        "orbital fixing statistics"
#define TABLE_POSITION_ORBITALFIXING    7001                    /**< the position of the statistics table */
#define TABLE_EARLIEST_ORBITALFIXING    SCIP_STAGE_SOLVING      /**< output of the statistics table is only printed from this stage onwards */


/*
 * Data structures
 */

/** propagator data for orbtial branching */
struct SCIP_PropData
{
   int                   npermvars;          /**< pointer to store number of variables for permutations */
   SCIP_VAR**            permvars;           /**< pointer to store variables on which permutations act */
   SCIP_HASHMAP*         permvarmap;         /**< map of variables to indices in permvars array */
   int                   nperms;             /**< pointer to store number of permutations */
   int**                 perms;              /**< pointer to store permutation generators as (nperms x npermvars) matrix */
   SCIP_Bool             enabled;            /**< run orbital branching? */
   int                   nfixedzero;         /**< number of variables fixed to 0 */
   int                   nfixedone;          /**< number of variables fixed to 1 */
   SCIP_Longint          nodenumber;         /**< number of node where propagation has been last applied */
   SCIP_PRESOL*          symmetrypresol;     /**< pointer to symmetry presolver */
};



/*
 * Table callback methods
 */

/** output method of orbital fixing propagator statistics table to output file stream 'file' */
static
SCIP_DECL_TABLEOUTPUT(tableOutputOrbitalfixing)
{
   SCIP_PROPDATA* propdata;

   assert( scip != NULL );
   assert( table != NULL );

   propdata = (SCIP_PROPDATA*) SCIPtableGetData(table);
   assert( propdata != NULL );

   if ( propdata->enabled )
   {
      SCIPverbMessage(scip, SCIP_VERBLEVEL_MINIMAL, file, "Orbital fixing     :\n");
      SCIPverbMessage(scip, SCIP_VERBLEVEL_MINIMAL, file, "  vars fixed to 0  :\t%7d\n", propdata->nfixedzero);
      SCIPverbMessage(scip, SCIP_VERBLEVEL_MINIMAL, file, "  vars fixed to 1  :\t%7d\n", propdata->nfixedone);
   }

   return SCIP_OKAY;
}


/*
 * Local methods
 */


/** perform orbital fixing
 *
 *  Note that we do not have to distinguish between variables that have been fixed or branched to 1, since the
 *  stabilizer is with respect to the variables that have been branched to 1. Thus, if an orbit contains a variable that
 *  has been branched to 1, the whole orbit only contains variables that have been branched to 1 - and nothing can be
 *  fixed.
 */
static
SCIP_RETCODE orbitalFixing(
   SCIP*                 scip,               /**< SCIP pointer */
   SCIP_VAR**            permvars,           /**< variables */
   int                   npermvars,          /**< number of variables */
   int*                  orbits,             /**< array of non-trivial orbits */
   int*                  orbitbegins,        /**< array containing begin positions of new orbits in orbits array */
   int                   norbits,            /**< number of orbits */
   SCIP_Bool*            infeasible,         /**< pointer to store whether problem is infeasible */
   int*                  nfixedzero,         /**< pointer to store number of variables fixed to 0 */
   int*                  nfixedone           /**< pointer to store number of variables fixed to 1 */
   )
{
   SCIP_Bool tightened;
   int i;

   assert( scip != NULL );
   assert( permvars != NULL );
   assert( orbits != NULL );
   assert( orbitbegins != NULL );
   assert( infeasible != NULL );
   assert( nfixedzero != NULL );
   assert( nfixedone != NULL );
   assert( norbits > 0 );
   assert( orbitbegins[0] == 0 );

   *infeasible = FALSE;
   *nfixedzero = 0;
   *nfixedone = 0;

   SCIPdebugMessage("Perform orbital fixing on %d orbits.\n", norbits);

   /* check all orbits */
   for (i = 0; i < norbits; ++i)
   {
      SCIP_Bool havefixedone = FALSE;
      SCIP_Bool havefixedzero = FALSE;
      SCIP_VAR* var;
      int j;

      /* we only have nontrivial orbits */
      assert( orbitbegins[i+1] - orbitbegins[i] >= 2 );

      /* check all variables in the orbit */
      for (j = orbitbegins[i]; j < orbitbegins[i+1]; ++j)
      {
         assert( 0 <= orbits[j] && orbits[j] < npermvars );
         var = permvars[orbits[j]];
         assert( var != NULL );

         /* check whether variable is not binary (and not implicit integer!) */
         if ( SCIPvarGetType(var) != SCIP_VARTYPE_BINARY )
         {
            /* skip orbit if there are non-binary variables */
            havefixedone = FALSE;
            havefixedzero = FALSE;
            break;
         }

         /* if variable is fixed to 1 -> can fix all variables in orbit to 1 */
         if ( SCIPvarGetLbLocal(var) > 0.5 )
            havefixedone = TRUE;

         /* check for zero-fixed variables */
         if ( SCIPvarGetUbLocal(var) < 0.5 )
            havefixedzero = TRUE;
      }

      /* check consistency */
      if ( havefixedone && havefixedzero )
      {
         *infeasible = TRUE;
         return SCIP_OKAY;
      }

      /* fix all variables to 0 if there is one variable fixed to 0 */
      if ( havefixedzero )
      {
         assert( ! havefixedone );

         for (j = orbitbegins[i]; j < orbitbegins[i+1]; ++j)
         {
            assert( 0 <= orbits[j] && orbits[j] < npermvars );
            var = permvars[orbits[j]];
            assert( var != NULL );

            /* only variables that are not yet fixed to 0 */
            if ( SCIPvarGetUbLocal(var) > 0.5 )
            {
               SCIPdebugMessage("can fix <%s> (index %d) to 0.\n", SCIPvarGetName(var), orbits[j]);
               assert( SCIPvarGetType(var) == SCIP_VARTYPE_BINARY );
               /* due to aggregation, var might already be fixed to 1, so do not put assert here */

               /* do not use SCIPinferBinvarProp(), since conflict analysis is not valid */
               SCIP_CALL( SCIPtightenVarUb(scip, var, 0.0, FALSE, infeasible, &tightened) );
               if ( *infeasible )
                  return SCIP_OKAY;
               if ( tightened )
                  ++(*nfixedzero);
            }
         }
      }

      /* fix all variables to 1 if there is one variable fixed to 1 */
      if ( havefixedone )
      {
         assert( ! havefixedzero );

         for (j = orbitbegins[i]; j < orbitbegins[i+1]; ++j)
         {
            assert( 0 <= orbits[j] && orbits[j] < npermvars );
            var = permvars[orbits[j]];
            assert( var != NULL );

            /* only variables that are not yet fixed to 1 */
            if ( SCIPvarGetLbLocal(var) < 0.5)
            {
               SCIPdebugMessage("can fix <%s> (index %d) to 1.\n", SCIPvarGetName(var), orbits[j]);
               assert( SCIPvarGetType(var) == SCIP_VARTYPE_BINARY );
               /* due to aggregation, var might already be fixed to 0, so do not put assert here */

               /* do not use SCIPinferBinvarProp(), since conflict analysis is not valid */
               SCIP_CALL( SCIPtightenVarLb(scip, var, 1.0, FALSE, infeasible, &tightened) );
               if ( *infeasible )
                  return SCIP_OKAY;
               if ( tightened )
                  ++(*nfixedone);
            }
         }
      }
   }

   return SCIP_OKAY;
}

/** Get branching variables on the path to root */
static
SCIP_RETCODE computeBranchingVariables(
   SCIP*                 scip,               /**< SCIP pointer */
   int                   nvars,              /**< number of variables */
   SCIP_HASHMAP*         varmap,             /**< map of variables to indices in vars array */
   SCIP_Shortbool*       b1                  /**< bitset marking the variables branched to 1 */
   )
{
   SCIP_NODE* node;

   assert( scip != NULL );
   assert( varmap != NULL );
   assert( b1 != NULL );

   /* get curent node */
   node = SCIPgetCurrentNode(scip);

#ifdef SCIP_OUTPUT
   SCIP_CALL( SCIPprintNodeRootPath(scip, node, NULL) );
#endif

   /* follow path to the root (in the root no domains were changed due to branching) */
   while ( SCIPnodeGetDepth(node) != 0 )
   {
      SCIP_BOUNDCHG* boundchg;
      SCIP_DOMCHG* domchg;
      SCIP_VAR* branchvar;
      int nboundchgs;
      int i;

      /* get domain changes of current node */
      domchg = SCIPnodeGetDomchg(node);
      assert( domchg != NULL );

      /* loop through all bound changes */
      nboundchgs = SCIPdomchgGetNBoundchgs(domchg);
      for (i = 0; i < nboundchgs; ++i)
      {
         /* get bound change info */
         boundchg = SCIPdomchgGetBoundchg(domchg, i);
         assert( boundchg != 0 );

         /* branching decisions have to be in the beginning of the bound change array */
         if ( SCIPboundchgGetBoundchgtype(boundchg) != SCIP_BOUNDCHGTYPE_BRANCHING )
            break;

         /* get corresponding branching variable */
         branchvar = SCIPboundchgGetVar(boundchg);

         /* we only consider binary variables */
         if ( SCIPvarIsBinary(branchvar) )
         {
            /* make sure that branching variable is known */
            assert( SCIPhashmapExists(varmap, (void*) branchvar) );

            if ( SCIPvarGetLbLocal(branchvar) > 0.5 )
            {
               int branchvaridx;

               branchvaridx = (int) (size_t) SCIPhashmapGetImage(varmap, (void*) branchvar);
               assert( branchvaridx < nvars );
               b1[branchvaridx] = TRUE;
            }
         }
      }

      node = SCIPnodeGetParent(node);
   }

   return SCIP_OKAY;
}


#ifndef NDEBUG
/** return objective coefficient, resolves negated or aggregated variables */
static
SCIP_Real varGetObjResolved(
   SCIP_VAR*             var                 /**< variable */
   )
{
   switch ( SCIPvarGetStatus(var) )
   {
   case SCIP_VARSTATUS_ORIGINAL:
   case SCIP_VARSTATUS_LOOSE:
   case SCIP_VARSTATUS_COLUMN:
      return SCIPvarGetObj(var);

   case SCIP_VARSTATUS_AGGREGATED:
      assert( SCIPvarGetAggrVar(var) != NULL );
      return SCIPvarGetAggrScalar(var) * SCIPvarGetObj(SCIPvarGetAggrVar(var));

   case SCIP_VARSTATUS_FIXED:
   {
      SCIP_RETCODE retcode;
      SCIP_Real scalar = 1.0;
      SCIP_Real constant = 0.0;
      SCIP_VAR* origvar;

      origvar = var;
      retcode = SCIPvarGetOrigvarSum(&origvar, &scalar, &constant);
      if ( retcode != SCIP_OKAY )
         return 0.0;
      return scalar * SCIPvarGetObj(origvar);
   }

   case SCIP_VARSTATUS_NEGATED:
      assert( SCIPvarIsNegated(var) );
      assert( SCIPvarGetNegatedVar(var) != NULL );
      return -SCIPvarGetObj(SCIPvarGetNegatedVar(var));

   case SCIP_VARSTATUS_MULTAGGR:
   default:
      break;
   }

   return 0.0;
}
#endif


/** propagate orbital fixing */
static
SCIP_RETCODE propagate(
   SCIP*                 scip,               /**< SCIP pointer */
   SCIP_PROPDATA*        propdata,           /**< propagator data */
   SCIP_Bool*            infeasible,         /**< pointer to store whether the node is detected to be infeasible */
   int*                  ngen                /**< pointer to store the number of propagations */
   )
{
   SCIP_Shortbool* activeperms;
   SCIP_Shortbool* b1;
   SCIP_VAR** permvars;
   int* orbitbegins;
   int* orbits;
   int norbits;
   int npermvars;
   int** perms;
   int nperms;
   int p;
   int v;

   assert( scip != NULL );
   assert( propdata != NULL );
   assert( infeasible != NULL );
   assert( ngen != NULL );

   *infeasible = FALSE;
   *ngen = 0;

   assert( propdata->permvars != NULL );
   assert( propdata->npermvars > 0 );
   assert( propdata->permvarmap != NULL );
   assert( propdata->perms != NULL );

   permvars = propdata->permvars;
   npermvars = propdata->npermvars;
   perms = propdata->perms;
   nperms = propdata->nperms;

   SCIP_CALL( SCIPallocBufferArray(scip, &b1, npermvars) );
   SCIP_CALL( SCIPallocBufferArray(scip, &activeperms, nperms) );
   for (v = 0; v < npermvars; ++v)
      b1[v] = FALSE;

   /* get branching variables */
   SCIP_CALL( computeBranchingVariables(scip, npermvars, propdata->permvarmap, b1) );

   /* filter out permutations that move variables that are fixed to different values */
   for (p = 0; p < nperms; ++p)
   {
      assert( perms[p] != NULL );

      for (v = 0; v < npermvars; ++v)
      {
         int img;

         img = perms[p][v];

         if ( img != v )
         {
            assert( SCIPvarGetType(permvars[v]) == SCIPvarGetType(permvars[img]) );
            assert( SCIPvarGetStatus(permvars[v]) == SCIP_VARSTATUS_MULTAGGR || SCIPvarGetStatus(permvars[img]) == SCIP_VARSTATUS_MULTAGGR ||
               SCIPisEQ(scip, varGetObjResolved(permvars[v]), varGetObjResolved(permvars[img])) );

            /* we are moving a variable branched to 1 to another variable */
            if ( b1[v] && ! b1[img] )
               break;
         }
      }

      if ( v >= npermvars )
         activeperms[p] = TRUE;
      else
         activeperms[p] = FALSE;
   }

   /* compute orbits */
   SCIP_CALL( SCIPallocBufferArray(scip, &orbits, npermvars) );
   SCIP_CALL( SCIPallocBufferArray(scip, &orbitbegins, npermvars) );
   SCIP_CALL( computeGroupOrbits(scip, permvars, npermvars, perms, nperms, activeperms, orbits, orbitbegins, &norbits) );

   SCIPfreeBufferArray(scip, &activeperms);
   SCIPfreeBufferArray(scip, &b1);

   if ( norbits > 0 )
   {
      int nfixedzero = 0;
      int nfixedone = 0;

      SCIP_CALL( orbitalFixing(scip, permvars, npermvars, orbits, orbitbegins, norbits, infeasible, &nfixedzero, &nfixedone) );

      propdata->nfixedzero += nfixedzero;
      propdata->nfixedone += nfixedone;
      *ngen = nfixedzero + nfixedone;

      SCIPdebugMessage("Orbital fixings: %d 0s, %d 1s.\n", nfixedzero, nfixedone);
   }

   SCIPfreeBufferArray(scip, &orbitbegins);
   SCIPfreeBufferArray(scip, &orbits);

   return SCIP_OKAY;
}




/*
 * Callback methods of propagator
 */


/** destructor of propagator to free user data (called when SCIP is exiting) */
static
SCIP_DECL_PROPFREE(propFreeOrbitalfixing)
{  /*lint --e{715,818}*/
   SCIP_PROPDATA* propdata;

   assert( prop != NULL );

   SCIPdebugMessage("Freeing propagator <%s> ...\n", SCIPpropGetName(prop));

   propdata = SCIPpropGetData(prop);
   assert( propdata != NULL );

   SCIPfreeBlockMemory(scip, &propdata);

   return SCIP_OKAY;
}


/** initialization method of propagator (called after problem was transformed) */
static
SCIP_DECL_PROPINIT(propInitOrbitalfixing)
{  /*lint --e{715}*/
   SCIP_PROPDATA* propdata;

   assert( prop != NULL );

   SCIPdebugMessage("Init propagator <%s> ...\n", SCIPpropGetName(prop));

   propdata = SCIPpropGetData(prop);
   assert( propdata != NULL );

   /* check whether we should run */
   SCIP_CALL( SCIPgetBoolParam(scip, "misc/usesymmetry", &propdata->enabled) );

   if ( propdata->enabled )
   {
      /* work on binary variables while fixing integer variables */
      SYMsetSpecRequirement(propdata->symmetrypresol, SYM_SPEC_BINARY);
      SYMsetSpecRequirement(propdata->symmetrypresol, SYM_SPEC_INTEGER);
   }

   return SCIP_OKAY;
}


/** deinitialization method of propagator (called before transformed problem is freed) */
static
SCIP_DECL_PROPEXIT(propExitOrbitalfixing)
{  /*lint --e{715}*/
   SCIP_PROPDATA* propdata;

   assert( prop != NULL );

   propdata = SCIPpropGetData(prop);
   assert( propdata != NULL );

   if ( propdata->permvarmap != NULL )
   {
      SCIPhashmapFree(&propdata->permvarmap);
   }

   /* reset paramters */
   propdata->nodenumber = -1;
   propdata->nfixedzero = 0;
   propdata->nfixedone = 0;

   propdata->permvars = NULL;
   propdata->npermvars = -1;
   propdata->permvarmap = NULL;

   return SCIP_OKAY;
}


/** solving process initialization method of propagator (called when branch and bound process is about to begin) */
static
SCIP_DECL_PROPINITSOL(propInitsolOrbitalfixing)
{  /*lint --e{715}*/
   SCIP_PROPDATA* propdata;

   assert( scip != NULL );
   assert( prop != NULL );

   /* get data */
   propdata = SCIPpropGetData(prop);
   assert( propdata != NULL );

   /* possibly skip orbital fixing */
   if ( ! propdata->enabled )
      return SCIP_OKAY;

   /* stop, if problem has already been solved */
   if ( SCIPgetStatus(scip) != SCIP_STATUS_UNKNOWN )
      return SCIP_OKAY;

   assert( SCIPisTransformed(scip) );

   /* possibly get symmetries */
   if ( propdata->npermvars < 0 )
   {
      SCIP_CALL( SCIPgetSymmetryGenerators(scip, propdata->symmetrypresol, &(propdata->npermvars),
            &(propdata->permvars), &(propdata->nperms), &(propdata->perms), NULL) );

      if ( propdata->nperms <= 0 )
      {
         SCIPverbMessage(scip, SCIP_VERBLEVEL_MINIMAL, 0, "Skip orbital fixing, since no symmetries were found.\n");
         propdata->enabled = FALSE;
      }
      else
      {
         int j;

         /* create hashmap for storing the indices of variables */
         assert( propdata->permvarmap == NULL );
         SCIP_CALL( SCIPhashmapCreate(&propdata->permvarmap, SCIPblkmem(scip), propdata->npermvars) );

         /* insert variables */
         for (j = 0; j < propdata->npermvars; ++j)
         {
            SCIP_CALL( SCIPhashmapInsert(propdata->permvarmap, propdata->permvars[j], (void*) (size_t) j) );
         }
      }
   }

   return SCIP_OKAY;
}


/** execution method of propagator */
static
SCIP_DECL_PROPEXEC(propExecOrbitalfixing)
{  /*lint --e{715}*/
   SCIP_PROPDATA* propdata;
   SCIP_Bool infeasible = FALSE;
   SCIP_Longint nn;
   int ngen = 0;

   assert( scip != NULL );
   assert( result != NULL );

   *result = SCIP_DIDNOTRUN;

   /* do not run if we are in the root or not yet solving */
   if ( SCIPgetDepth(scip) <= 0 || SCIPgetStage(scip) < SCIP_STAGE_SOLVING )
      return SCIP_OKAY;

   /* do nothing if we are in a probing node */
   if ( SCIPinProbing(scip) )
      return SCIP_OKAY;

   /* get data */
   propdata = SCIPpropGetData(prop);
   assert( propdata != NULL );

   /* do not run if not enabled */
   if ( ! propdata->enabled )
      return SCIP_OKAY;

   /* return if there is no symmetry available */
   if ( propdata->npermvars == 0 || propdata->permvars == NULL )
      return SCIP_OKAY;

   /* return if we already ran in this node */
   nn = SCIPnodeGetNumber(SCIPgetCurrentNode(scip));
   if ( nn == propdata->nodenumber )
      return SCIP_OKAY;
   propdata->nodenumber = nn;

   /* propagate */
   *result = SCIP_DIDNOTFIND;

   SCIPdebugMessage("Propagating <%s>.\n", SCIPpropGetName(prop));
   SCIP_CALL( propagate(scip, propdata, &infeasible, &ngen) );
   if ( infeasible )
      *result = SCIP_CUTOFF;
   else if ( ngen > 0 )
      *result = SCIP_REDUCEDDOM;

   return SCIP_OKAY;
}


/** propagation conflict resolving method of propagator
 *
 *  @todo Implement reverse propagation.
 */
static
SCIP_DECL_PROPRESPROP(propRespropOrbitalfixing)
{  /*lint --e{715,818}*/
   assert( result != NULL );

   *result = SCIP_DIDNOTFIND;

   return SCIP_OKAY;
}


/** creates the orbitalfixing propagator and includes it in SCIP */
SCIP_RETCODE SCIPincludePropOrbitalfixing(
   SCIP*                 scip                /**< SCIP data structure */
   )
{
   SCIP_PROPDATA* propdata;
   SCIP_PRESOL* presol;
   SCIP_PROP* prop;

   /* create orbitalfixing propagator data */
   SCIP_CALL( SCIPallocBlockMemory(scip, &propdata) );

   propdata->nodenumber = -1;
   propdata->nfixedzero = 0;
   propdata->nfixedone = 0;

   propdata->permvars = NULL;
   propdata->npermvars = -1;
   propdata->permvarmap = NULL;

   /* determine cons_symmetries constraint handler (preuse presol) */
   presol = SCIPfindPresol(scip, "symmetry");
   if ( presol == 0 )
   {
      SCIPerrorMessage("Could not find symmetry presolver.\n");
      return SCIP_PLUGINNOTFOUND;
   }
   propdata->symmetrypresol = presol;

   /* include propagator */
   SCIP_CALL( SCIPincludePropBasic(scip, &prop, PROP_NAME, PROP_DESC, PROP_PRIORITY, PROP_FREQ, PROP_DELAY, PROP_TIMING, propExecOrbitalfixing, propdata) );

   /* set callbacks */
   SCIP_CALL( SCIPsetPropFree(scip, prop, propFreeOrbitalfixing) );
   SCIP_CALL( SCIPsetPropInit(scip, prop, propInitOrbitalfixing) );
   SCIP_CALL( SCIPsetPropExit(scip, prop, propExitOrbitalfixing) );
   SCIP_CALL( SCIPsetPropInitsol(scip, prop, propInitsolOrbitalfixing) );
   SCIP_CALL( SCIPsetPropResprop(scip, prop, propRespropOrbitalfixing) );

   /* include table */
   SCIP_CALL( SCIPincludeTable(scip, TABLE_NAME_ORBITALFIXING, TABLE_DESC_ORBITALFIXING, TRUE,
         NULL, NULL, NULL, NULL, NULL, NULL, tableOutputOrbitalfixing,
         (void*) propdata, TABLE_POSITION_ORBITALFIXING, TABLE_EARLIEST_ORBITALFIXING) );

   return SCIP_OKAY;
}
