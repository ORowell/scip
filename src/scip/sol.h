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
#pragma ident "@(#) $Id: sol.h,v 1.32 2004/10/05 11:01:39 bzfpfend Exp $"

/**@file   sol.h
 * @brief  internal methods for storing primal CIP solutions
 * @author Tobias Achterberg
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __SOL_H__
#define __SOL_H__


#include <stdio.h>

#include "def.h"
#include "memory.h"
#include "type_retcode.h"
#include "type_set.h"
#include "type_stat.h"
#include "type_lp.h"
#include "type_var.h"
#include "type_prob.h"
#include "type_sol.h"
#include "type_primal.h"
#include "type_tree.h"
#include "type_heur.h"
#include "pub_sol.h"




/** creates primal CIP solution, initialized to zero */
extern
RETCODE SCIPsolCreate(
   SOL**            sol,                /**< pointer to primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   PRIMAL*          primal,             /**< primal data */
   TREE*            tree,               /**< branch and bound tree, or NULL */
   HEUR*            heur                /**< heuristic that found the solution (or NULL if it's from the tree) */
   );

/** creates a copy of a primal CIP solution */
extern
RETCODE SCIPsolCopy(
   SOL**            sol,                /**< pointer to store the copy of the primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   PRIMAL*          primal,             /**< primal data */
   SOL*             sourcesol           /**< primal CIP solution to copy */
   );

/** creates primal CIP solution, initialized to the current LP solution */
extern
RETCODE SCIPsolCreateLPSol(
   SOL**            sol,                /**< pointer to primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   PRIMAL*          primal,             /**< primal data */
   TREE*            tree,               /**< branch and bound tree */
   LP*              lp,                 /**< current LP data */
   HEUR*            heur                /**< heuristic that found the solution (or NULL if it's from the tree) */
   );

/** creates primal CIP solution, initialized to the current pseudo solution */
extern
RETCODE SCIPsolCreatePseudoSol(
   SOL**            sol,                /**< pointer to primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   PRIMAL*          primal,             /**< primal data */
   TREE*            tree,               /**< branch and bound tree */
   LP*              lp,                 /**< current LP data */
   HEUR*            heur                /**< heuristic that found the solution (or NULL if it's from the tree) */
   );

/** creates primal CIP solution, initialized to the current solution */
extern
RETCODE SCIPsolCreateCurrentSol(
   SOL**            sol,                /**< pointer to primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   PRIMAL*          primal,             /**< primal data */
   TREE*            tree,               /**< branch and bound tree */
   LP*              lp,                 /**< current LP data */
   HEUR*            heur                /**< heuristic that found the solution (or NULL if it's from the tree) */
   );

/** frees primal CIP solution */
extern
RETCODE SCIPsolFree(
   SOL**            sol,                /**< pointer to primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   PRIMAL*          primal              /**< primal data */
   );

/** informs the solution that it now belongs to the given primal heuristic */
extern
void SCIPsolSetHeur(
   SOL*             sol,                /**< primal CIP solution */
   HEUR*            heur                /**< heuristic that found the solution (or NULL if it's from the tree) */
   );

/** copies current LP solution into CIP solution by linking */
extern
RETCODE SCIPsolLinkLPSol(
   SOL*             sol,                /**< primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   TREE*            tree,               /**< branch and bound tree */
   LP*              lp                  /**< current LP data */
   );

/** copies current pseudo solution into CIP solution by linking */
extern
RETCODE SCIPsolLinkPseudoSol(
   SOL*             sol,                /**< primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   TREE*            tree,               /**< branch and bound tree */
   LP*              lp                  /**< current LP data */
   );

/** copies current solution (LP or pseudo solution) into CIP solution by linking */
extern
RETCODE SCIPsolLinkCurrentSol(
   SOL*             sol,                /**< primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   TREE*            tree,               /**< branch and bound tree */
   LP*              lp                  /**< current LP data */
   );

/** clears primal CIP solution */
extern
RETCODE SCIPsolClear(
   SOL*             sol,                /**< primal CIP solution */
   STAT*            stat,               /**< problem statistics data */
   TREE*            tree                /**< branch and bound tree */
   );

/** stores solution values of variables in solution's own array */
extern
RETCODE SCIPsolUnlink(
   SOL*             sol,                /**< primal CIP solution */
   SET*             set,                /**< global SCIP settings */
   PROB*            prob                /**< problem data */
   );

/** sets value of variable in primal CIP solution */
extern
RETCODE SCIPsolSetVal(
   SOL*             sol,                /**< primal CIP solution */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   TREE*            tree,               /**< branch and bound tree */
   VAR*             var,                /**< variable to add to solution */
   Real             val                 /**< solution value of variable */
   );

/** increases value of variable in primal CIP solution */
extern
RETCODE SCIPsolIncVal(
   SOL*             sol,                /**< primal CIP solution */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   TREE*            tree,               /**< branch and bound tree */
   VAR*             var,                /**< variable to increase solution value for */
   Real             incval              /**< increment for solution value of variable */
   );

/** returns value of variable in primal CIP solution */
extern
Real SCIPsolGetVal(
   SOL*             sol,                /**< primal CIP solution */
   STAT*            stat,               /**< problem statistics data */
   VAR*             var                 /**< variable to get value for */
   );

/** updates primal solutions after a change in a variable's objective value */
extern
void SCIPsolUpdateVarObj(
   SOL*             sol,                /**< primal CIP solution */
   VAR*             var,                /**< problem variable */
   Real             oldobj,             /**< old objective value */
   Real             newobj              /**< new objective value */
   );

/** checks primal CIP solution for feasibility */
extern
RETCODE SCIPsolCheck(
   SOL*             sol,                /**< primal CIP solution */
   MEMHDR*          memhdr,             /**< block memory */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics */
   PROB*            prob,               /**< problem data */
   Bool             checkintegrality,   /**< has integrality to be checked? */
   Bool             checklprows,        /**< have current LP rows to be checked? */
   Bool*            feasible            /**< stores whether solution is feasible */
   );

/** try to round given solution */
extern
RETCODE SCIPsolRound(
   SOL*             sol,                /**< primal solution */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   PROB*            prob,               /**< problem data */
   TREE*            tree,               /**< branch and bound tree */
   Bool*            success             /**< pointer to store whether rounding was successful */
   );

/** updates the solution value sums in variables by adding the value in the given solution */
extern
void SCIPsolUpdateVarsum(
   SOL*             sol,                /**< primal CIP solution */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   PROB*            prob,               /**< transformed problem data */
   Real             weight              /**< weight of solution in weighted average */
   );

/** outputs non-zero elements of solution to file stream */
extern
RETCODE SCIPsolPrint(
   SOL*             sol,                /**< primal CIP solution */
   SET*             set,                /**< global SCIP settings */
   STAT*            stat,               /**< problem statistics data */
   PROB*            prob,               /**< problem data (original or transformed) */
   PROB*            transprob,          /**< transformed problem data or NULL (to display priced variables) */
   FILE*            file                /**< output file (or NULL for standard output) */
   );


#ifndef NDEBUG

/* In debug mode, the following methods are implemented as function calls to ensure
 * type validity.
 */

/** gets current position of solution in array of existing solutions of primal data */
extern
int SCIPsolGetPrimalIndex(
   SOL*             sol                 /**< primal CIP solution */
   );

/** sets current position of solution in array of existing solutions of primal data */
extern
void SCIPsolSetPrimalIndex(
   SOL*             sol,                /**< primal CIP solution */
   int              primalindex         /**< new primal index of solution */
   );

#else

/* In optimized mode, the methods are implemented as defines to reduce the number of function calls and
 * speed up the algorithms.
 */

#define SCIPsolGetPrimalIndex(sol)      ((sol)->primalindex)
#define SCIPsolSetPrimalIndex(sol,idx)  { (sol)->primalindex = idx; }

#endif


#endif
