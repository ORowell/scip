/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2004 Tobias Achterberg                              */
/*                                                                           */
/*                  2002-2004 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the SCIP Academic Licence.        */
/*                                                                           */
/*  You should have received a copy of the SCIP Academic License             */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma ident "@(#) $Id: pub_cons.h,v 1.10 2004/07/06 17:04:15 bzfpfend Exp $"

/**@file   pub_cons.h
 * @brief  public methods for managing constraints
 * @author Tobias Achterberg
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __PUB_CONS_H__
#define __PUB_CONS_H__


#include "def.h"
#include "type_misc.h"
#include "type_cons.h"

#ifdef NDEBUG
#include "struct_cons.h"
#endif



/*
 * Constraint handler methods
 */

/** compares two constraint handlers w. r. to their relaxation and separation priority */
extern
DECL_SORTPTRCOMP(SCIPconshdlrCompSepa);

/** compares two constraint handlers w. r. to their enforcing priority */
extern
DECL_SORTPTRCOMP(SCIPconshdlrCompEnfo);

/** compares two constraint handlers w. r. to their feasibility check priority */
extern
DECL_SORTPTRCOMP(SCIPconshdlrCompCheck);

/** gets name of constraint handler */
extern
const char* SCIPconshdlrGetName(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets description of constraint handler */
extern
const char* SCIPconshdlrGetDesc(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets user data of constraint handler */
extern
CONSHDLRDATA* SCIPconshdlrGetData(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** sets user data of constraint handler; user has to free old data in advance! */
extern
void SCIPconshdlrSetData(
   CONSHDLR*        conshdlr,           /**< constraint handler */
   CONSHDLRDATA*    conshdlrdata        /**< new constraint handler user data */
   );

/** gets array with active constraints of constraint handler */
extern
CONS** SCIPconshdlrGetConss(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of active constraints of constraint handler */
extern
int SCIPconshdlrGetNConss(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of enabled constraints of constraint handler */
extern
int SCIPconshdlrGetNEnabledConss(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets time in seconds used for presolving in this constraint handler */
extern
Real SCIPconshdlrGetPresolTime(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets time in seconds used for relaxation in this constraint handler */
extern
Real SCIPconshdlrGetRelaxTime(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets time in seconds used for separation in this constraint handler */
extern
Real SCIPconshdlrGetSepaTime(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets time in seconds used for LP enforcement in this constraint handler */
extern
Real SCIPconshdlrGetEnfoLPTime(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets time in seconds used for pseudo enforcement in this constraint handler */
extern
Real SCIPconshdlrGetEnfoPSTime(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets time in seconds used for propagation in this constraint handler */
extern
Real SCIPconshdlrGetPropTime(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of calls to the constraint handler's relaxation method */
extern
Longint SCIPconshdlrGetNRelaxCalls(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of calls to the constraint handler's separation method */
extern
Longint SCIPconshdlrGetNSepaCalls(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of calls to the constraint handler's LP enforcing method */
extern
Longint SCIPconshdlrGetNEnfoLPCalls(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of calls to the constraint handler's pseudo enforcing method */
extern
Longint SCIPconshdlrGetNEnfoPSCalls(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of calls to the constraint handler's propagation method */
extern
Longint SCIPconshdlrGetNPropCalls(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets total number of times, this constraint handler detected a cutoff */
extern
Longint SCIPconshdlrGetNCutoffs(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets total number of cuts found by this constraint handler */
extern
Longint SCIPconshdlrGetNCutsFound(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets total number of additional constraints added by this constraint handler */
extern
Longint SCIPconshdlrGetNConssFound(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets total number of domain reductions found by this constraint handler */
extern
Longint SCIPconshdlrGetNDomredsFound(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of children created by this constraint handler */
extern
Longint SCIPconshdlrGetNChildren(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets maximum number of active constraints of constraint handler existing at the same time */
extern
int SCIPconshdlrGetMaxNConss(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets initial number of active constraints of constraint handler */
extern
int SCIPconshdlrGetStartNConss(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of variables fixed in presolving method of constraint handler */
extern
int SCIPconshdlrGetNFixedVars(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of variables aggregated in presolving method of constraint handler */
extern
int SCIPconshdlrGetNAggrVars(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of variable types changed in presolving method of constraint handler */
extern
int SCIPconshdlrGetNVarTypes(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of bounds changed in presolving method of constraint handler */
extern
int SCIPconshdlrGetNChgBds(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of holes added to domains of variables in presolving method of constraint handler */
extern
int SCIPconshdlrGetNAddHoles(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of constraints deleted in presolving method of constraint handler */
extern
int SCIPconshdlrGetNDelConss(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of constraints upgraded in presolving method of constraint handler */
extern
int SCIPconshdlrGetNUpgdConss(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of coefficients changed in presolving method of constraint handler */
extern
int SCIPconshdlrGetNChgCoefs(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets number of constraint sides changed in presolving method of constraint handler */
extern
int SCIPconshdlrGetNChgSides(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets relaxation and separation priority of constraint handler */
extern
int SCIPconshdlrGetSepaPriority(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets enforcing priority of constraint handler */
extern
int SCIPconshdlrGetEnfoPriority(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets checking priority of constraint handler */
extern
int SCIPconshdlrGetCheckPriority(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets relaxation frequency of constraint handler */
extern
int SCIPconshdlrGetRelaxFreq(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets separation frequency of constraint handler */
extern
int SCIPconshdlrGetSepaFreq(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets propagation frequency of constraint handler */
extern
int SCIPconshdlrGetPropFreq(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** gets frequency of constraint handler for eager evaluations in relaxation, separation, propagation and enforcement */
extern
int SCIPconshdlrGetEagerFreq(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** needs constraint handler a constraint to be called? */
extern
Bool SCIPconshdlrNeedsCons(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** does the constraint handler perform presolving? */
extern
Bool SCIPconshdlrDoesPresolve(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );

/** is constraint handler initialized? */
extern
Bool SCIPconshdlrIsInitialized(
   CONSHDLR*        conshdlr            /**< constraint handler */
   );




/*
 * Constraint methods
 */

#ifndef NDEBUG

/* In debug mode, the following methods are implemented as function calls to ensure
 * type validity.
 */

/** returns the name of the constraint */
extern
const char* SCIPconsGetName(
   CONS*            cons                /**< constraint */
   );

/** returns the constraint handler of the constraint */
extern
CONSHDLR* SCIPconsGetHdlr(
   CONS*            cons                /**< constraint */
   );

/** returns the constraint data field of the constraint */
extern
CONSDATA* SCIPconsGetData(
   CONS*            cons                /**< constraint */
   );

/** gets number of times, the constraint is currently captured */
extern
int SCIPconsGetNUses(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is active in the current node */
extern
Bool SCIPconsIsActive(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is enabled in the current node */
extern
Bool SCIPconsIsEnabled(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is deleted or marked to be deleted */
extern
Bool SCIPconsIsDeleted(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is marked obsolete */
extern
Bool SCIPconsIsObsolete(
   CONS*            cons                /**< constraint */
   );

/** gets age of constraint */
extern
Real SCIPconsGetAge(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff the LP relaxation of constraint should be in the initial LP */
extern
Bool SCIPconsIsInitial(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint should be relaxed during LP processing */
extern
Bool SCIPconsIsRelaxed(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint should be separated during LP processing */
extern
Bool SCIPconsIsSeparated(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint should be enforced during node processing */
extern
Bool SCIPconsIsEnforced(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint should be checked for feasibility */
extern
Bool SCIPconsIsChecked(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint should be propagated during node processing */
extern
Bool SCIPconsIsPropagated(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is globally valid */
extern
Bool SCIPconsIsGlobal(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is only locally valid or not added to any (sub)problem */
extern
Bool SCIPconsIsLocal(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is modifiable (subject to column generation) */
extern
Bool SCIPconsIsModifiable(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint should be removed from the LP due to aging or cleanup */
extern
Bool SCIPconsIsRemoveable(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint belongs to the global problem */
extern
Bool SCIPconsIsInProb(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is belonging to original space */
extern
Bool SCIPconsIsOriginal(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff constraint is belonging to transformed space */
extern
Bool SCIPconsIsTransformed(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff roundings for variables in constraint are locked */
extern
Bool SCIPconsIsLockedPos(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff roundings for variables in constraint's negation are locked */
extern
Bool SCIPconsIsLockedNeg(
   CONS*            cons                /**< constraint */
   );

/** returns TRUE iff roundings for variables in constraint or in constraint's negation are locked */
extern
Bool SCIPconsIsLocked(
   CONS*            cons                /**< constraint */
   );

#else

/* In optimized mode, the methods are implemented as defines to reduce the number of function calls and
 * speed up the algorithms.
 */

#define SCIPconsGetName(cons)           (cons)->name
#define SCIPconsGetHdlr(cons)           (cons)->conshdlr
#define SCIPconsGetData(cons)           (cons)->consdata
#define SCIPconsGetNUses(cons)          (cons)->nuses
#define SCIPconsIsActive(cons)          ((cons)->updateactivate || ((cons)->active && !(cons)->updatedeactivate))
#define SCIPconsIsEnabled(cons)         ((cons)->updateenable || ((cons)->enabled && !(cons)->updatedisable))
#define SCIPconsIsDeleted(cons)         ((cons)->deleted || (cons)->updatedelete)
#define SCIPconsIsObsolete(cons)        ((cons)->updateobsolete || (cons)->obsolete)
#define SCIPconsGetAge(cons)            (cons)->age
#define SCIPconsIsInitial(cons)         (cons)->initial
#define SCIPconsIsRelaxed(cons)         (cons)->relax
#define SCIPconsIsSeparated(cons)       (cons)->separate
#define SCIPconsIsEnforced(cons)        (cons)->enforce
#define SCIPconsIsChecked(cons)         (cons)->check
#define SCIPconsIsPropagated(cons)      (cons)->propagate
#define SCIPconsIsGlobal(cons)          !(cons)->local
#define SCIPconsIsLocal(cons)           (cons)->local
#define SCIPconsIsModifiable(cons)      (cons)->modifiable
#define SCIPconsIsRemoveable(cons)      (cons)->removeable
#define SCIPconsIsInProb(cons)          ((cons)->addconssetchg == NULL && (cons)->addarraypos >= 0)
#define SCIPconsIsOriginal(cons)        (cons)->original
#define SCIPconsIsTransformed(cons)     !(cons)->original
#define SCIPconsIsLockedPos(cons)       ((cons)->nlockspos > 0)
#define SCIPconsIsLockedNeg(cons)       ((cons)->nlocksneg > 0)
#define SCIPconsIsLocked(cons)          ((cons)->nlockspos > 0 || (cons)->nlocksneg > 0)

#endif



#endif
