/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2016 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the ZIB Academic License.         */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file   branch_lookahead.c
 * @brief  lookahead branching rule
 * @author Christoph Schubert
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#include <assert.h>
#include <string.h>

#include "scip/branch_lookahead.h"
#include "scip/branch_fullstrong.h"

#define BRANCHRULE_NAME            "lookahead"
#define BRANCHRULE_DESC            "fullstrong branching with depth of 2" /* TODO CS: expand description */
#define BRANCHRULE_PRIORITY        1000000000
#define BRANCHRULE_MAXDEPTH        -1
#define BRANCHRULE_MAXBOUNDDIST    1.0


#define DEFAULT_MAXPROPROUNDS       0        /**< maximum number of propagation rounds to be performed during multaggr branching
                                              *   before solving the LP (-1: no limit, -2: parameter settings) */
#define DEFAULT_PROBINGBOUNDS    TRUE        /**< should valid bounds be identified in a probing-like fashion during multi-aggr
                                              *   branching (only with propagation)? */

/*
 * Data structures
 */

/* TODO: fill in the necessary branching rule data */

/** branching rule data */
struct SCIP_BranchruleData
{
   SCIP_Bool             probingbounds;      /**< should valid bounds be identified in a probing-like fashion during strong
                                              *   branching (only with propagation)? */
   int                   lastcand;           /**< last evaluated candidate of last branching rule execution */
   int                   maxproprounds;      /**< maximum number of propagation rounds to be performed during strong branching
                                              *   before solving the LP (-1: no limit, -2: parameter settings) */
   SCIP_Bool*            skipdown;           /**< should be branching on down child be skipped? */
   SCIP_Bool*            skipup;             /**< should be branching on up child be skipped? */
};


/*
 * Local methods
 */

/* put your local methods here, and declare them static */


/*
 * Callback methods of branching rule
 */


/** copy method for branchrule plugins (called when SCIP copies plugins) */
static
SCIP_DECL_BRANCHCOPY(branchCopyLookahead)
{  /*lint --e{715}*/
   SCIPerrorMessage("method of lookahead branching rule not implemented yet\n");
   SCIPABORT(); /*lint --e{527}*/

   return SCIP_OKAY;
}

/** destructor of branching rule to free user data (called when SCIP is exiting) */
static
SCIP_DECL_BRANCHFREE(branchFreeLookahead)
{  /*lint --e{715}*/
   SCIPerrorMessage("method of lookahead branching rule not implemented yet\n");
   SCIPABORT(); /*lint --e{527}*/

   return SCIP_OKAY;
}


/** initialization method of branching rule (called after problem was transformed) */
static
SCIP_DECL_BRANCHINIT(branchInitLookahead)
{  /*lint --e{715}*/
   SCIPerrorMessage("method of lookahead branching rule not implemented yet\n");
   SCIPABORT(); /*lint --e{527}*/

   return SCIP_OKAY;
}


/** deinitialization method of branching rule (called before transformed problem is freed) */
static
SCIP_DECL_BRANCHEXIT(branchExitLookahead)
{  /*lint --e{715}*/
   SCIPerrorMessage("method of lookahead branching rule not implemented yet\n");
   SCIPABORT(); /*lint --e{527}*/

   return SCIP_OKAY;
}


/** branching execution method for fractional LP solutions */
static
SCIP_DECL_BRANCHEXECLP(branchExeclpLookahead)
{  /*lint --e{715}*/

   SCIP_BRANCHRULEDATA* branchruledata;
   SCIP_VAR** tmplpcands;
   SCIP_VAR** lpcands;
   SCIP_Real* tmplpcandssol;
   SCIP_Real* tmplpcandsfrac;
   SCIP_Real* lpcandsfrac;
   SCIP_Real* lpcandssol;
   SCIP_Real bestup;
   SCIP_Real bestdown;
   SCIP_Real bestscore;
   SCIP_Real provedbound;
   SCIP_Bool bestdownvalid;
   SCIP_Bool bestupvalid;
   int nlpcands;
   int npriolpcands;
   int bestcandpos;

   assert(branchrule != NULL);
   assert(strcmp(SCIPbranchruleGetName(branchrule), BRANCHRULE_NAME) == 0);
   assert(scip != NULL);
   assert(result != NULL);

   SCIPdebugMessage("Execlp method of lookahead branching\n ");
   *result = SCIP_DIDNOTRUN;

   branchruledata = SCIPbranchruleGetData(branchrule);
   assert(branchruledata != NULL);

   SCIP_CALL( SCIPgetLPBranchCands(scip, &tmplpcands, &tmplpcandssol, &tmplpcandsfrac, &nlpcands, &npriolpcands, NULL) );
   assert(nlpcands > 0);
   assert(npriolpcands > 0);

   /* copy LP branching candidates and solution values, because they will be updated w.r.t. the strong branching LP
    * solution
    */
   SCIP_CALL( SCIPduplicateBufferArray(scip, &lpcands, tmplpcands, nlpcands) );
   SCIP_CALL( SCIPduplicateBufferArray(scip, &lpcandssol, tmplpcandssol, nlpcands) );
   SCIP_CALL( SCIPduplicateBufferArray(scip, &lpcandsfrac, tmplpcandsfrac, nlpcands) );

   if( branchruledata->skipdown == NULL )
   {
      int nvars = SCIPgetNVars(scip);

      assert(branchruledata->skipup == NULL);

      SCIP_CALL( SCIPallocMemoryArray(scip, &branchruledata->skipdown, nvars) );
      SCIP_CALL( SCIPallocMemoryArray(scip, &branchruledata->skipup, nvars) );
      BMSclearMemoryArray(branchruledata->skipdown, nvars);
      BMSclearMemoryArray(branchruledata->skipup, nvars);
   }

   /* compute strong branching among the array of fractional variables in order to get the best one */
   SCIP_CALL( SCIPselectVarStrongBranching(scip, lpcands, lpcandssol, lpcandsfrac, branchruledata->skipdown,
         branchruledata->skipup, nlpcands, npriolpcands, nlpcands, &branchruledata->lastcand, allowaddcons,
         branchruledata->maxproprounds, branchruledata->probingbounds, TRUE,
         &bestcandpos, &bestdown, &bestup, &bestscore, &bestdownvalid, &bestupvalid, &provedbound, result) );

   if( *result != SCIP_CUTOFF && *result != SCIP_REDUCEDDOM && *result != SCIP_CONSADDED )
   {
      SCIP_VAR* bestcand = lpcands[bestcandpos];
      SCIP_Real bestsol = lpcandssol[bestcandpos];

   }

   SCIPerrorMessage("method of lookahead branching rule not implemented yet\n");
   SCIPABORT(); /*lint --e{527}*/

   return SCIP_OKAY;
}

/*
 * branching rule specific interface methods
 */

/** creates the lookahead branching rule and includes it in SCIP */
SCIP_RETCODE SCIPincludeBranchruleLookahead(
   SCIP*                 scip                /**< SCIP data structure */
   )
{
   SCIP_BRANCHRULEDATA* branchruledata;
   SCIP_BRANCHRULE* branchrule;

   /* create lookahead branching rule data */
   SCIP_CALL( SCIPallocMemory(scip, &branchruledata) );
   branchruledata->lastcand = 0;
   branchruledata->skipup = NULL;
   branchruledata->skipdown = NULL;
   /* TODO: (optional) create branching rule specific data here */

   /* include branching rule */
   SCIP_CALL( SCIPincludeBranchruleBasic(scip, &branchrule, BRANCHRULE_NAME, BRANCHRULE_DESC, BRANCHRULE_PRIORITY,
         BRANCHRULE_MAXDEPTH, BRANCHRULE_MAXBOUNDDIST, branchruledata) );

   assert(branchrule != NULL);

   /* set non fundamental callbacks via setter functions */
   SCIP_CALL( SCIPsetBranchruleCopy(scip, branchrule, branchCopyLookahead) );
   SCIP_CALL( SCIPsetBranchruleFree(scip, branchrule, branchFreeLookahead) );
   SCIP_CALL( SCIPsetBranchruleInit(scip, branchrule, branchInitLookahead) );
   SCIP_CALL( SCIPsetBranchruleExit(scip, branchrule, branchExitLookahead) );
   SCIP_CALL( SCIPsetBranchruleExecLp(scip, branchrule, branchExeclpLookahead) );

   /* add lookahead branching rule parameters */
   SCIP_CALL( SCIPaddIntParam(scip,
         "branching/lookahead/maxproprounds",
         "maximum number of propagation rounds to be performed during lookahead branching before solving the LP (-1: no limit, -2: parameter settings)",
         &branchruledata->maxproprounds, TRUE, DEFAULT_MAXPROPROUNDS, -2, INT_MAX, NULL, NULL) );
   SCIP_CALL( SCIPaddBoolParam(scip,
         "branching/lookahead/probingbounds",
         "should valid bounds be identified in a probing-like fashion during lookahead branching (only with propagation)?",
         &branchruledata->probingbounds, TRUE, DEFAULT_PROBINGBOUNDS, NULL, NULL) );

   return SCIP_OKAY;
}
