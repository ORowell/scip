/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2002 Tobias Achterberg                              */
/*                            Thorsten Koch                                  */
/*                            Alexander Martin                               */
/*                  2002-2002 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the SCIP Academic Licence.        */
/*                                                                           */
/*  You should have received a copy of the SCIP Academic License             */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file   scip.h
 * @brief  SCIP callable library
 * @author Tobias Achterberg
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __SCIP_H__
#define __SCIP_H__


/** SCIP operation stage */
enum Stage
{
   SCIP_STAGE_UNINIT  = 0,              /**< SCIP datastructures are not initialized */
   SCIP_STAGE_INIT    = 1,              /**< SCIP datastructures are initialized, no problem exists */
   SCIP_STAGE_PROBLEM = 2,              /**< the problem is being created and modified */
   SCIP_STAGE_SOLVING = 3,              /**< the problem is being solved */
   SCIP_STAGE_SOLVED  = 4               /**< the problem was solved */
};
typedef enum Stage STAGE;


typedef struct Scip SCIP;               /**< SCIP main data structure */


#include <stdio.h>

#include "def.h"
#include "retcode.h"
#include "memory.h"
#include "message.h"
#include "cons.h"
#include "var.h"
#include "lp.h"
#include "tree.h"
#include "nodesel.h"


#define CHECK_SCIP(x) { RETCODE _retcode_; \
                        if( (_retcode_ = (x)) != SCIP_OKAY ) \
                          SCIPerror(stderr, _retcode_, __FILE__, __LINE__); \
                      }

#define SCIPallocBlockMemory(scip,ptr)          allocBlockMemory(SCIPmemhdr(scip), (ptr))
#define SCIPallocBlockMemoryArray(scip,ptr,num) allocBlockMemoryArray(SCIPmemhdr(scip), (ptr), (num))
#define SCIPallocBlockMemorySize(scip,ptr,size) allocBlockMemorySize(SCIPmemhdr(scip), (ptr), (size))
#define SCIPreallocBlockMemoryArray(scip,ptr,oldnum,newnum) \
                                                reallocBlockMemoryArray(SCIPmemhdr(scip), \
                                                  (ptr), (oldnum), (newnum))
#define SCIPreallocBlockMemorySize(scip,ptr,oldsize,newsize) \
                                                reallocBlockMemorySize(SCIPmemhdr(scip), \
                                                  (ptr), (oldsize), (newsize))
#define SCIPduplicateBlockMemory(scip, ptr, source) \
                                                duplicateBlockMemory(SCIPmemhdr(scip), (ptr), (source))
#define SCIPduplicateBlockMemoryArray(scip, ptr, source, num) \
                                                duplicateBlockMemoryArray(SCIPmemhdr(scip), (ptr), (source), (num))
#define SCIPfreeBlockMemory(scip,ptr)           freeBlockMemory(SCIPmemhdr(scip), (ptr))
#define SCIPfreeBlockMemoryNull(scip,ptr)       freeBlockMemoryNull(SCIPmemhdr(scip), (ptr))
#define SCIPfreeBlockMemoryArray(scip,ptr,num)  freeBlockMemoryArray(SCIPmemhdr(scip), (ptr), (num))
#define SCIPfreeBlockMemoryArrayNull(scip,ptr,num) \
                                                freeBlockMemoryArrayNull(SCIPmemhdr(scip), (ptr), (num))
#define SCIPfreeBlockMemorySize(scip,ptr,size)  freeBlockMemorySize(SCIPmemhdr(scip), (ptr), (size))
#define SCIPfreeBlockMemorySizeNull(scip,ptr,size) \
                                                freeBlockMemorySizeNull(SCIPmemhdr(scip), (ptr), (size))


extern
Real SCIPversion(                       /**< returns scip version number */
   void
   );

extern
void SCIPerror(                         /**< prints error message and aborts program execution */
   FILE*            errout,             /**< file stream to write error message */
   RETCODE          retcode,            /**< SCIP return code causing the error */
   const char*      filename,           /**< source code file name */
   int              line                /**< source line */
   );

extern
RETCODE SCIPcreate(                     /**< creates and initializes SCIP data structures */
   SCIP**           scip                /**< pointer to SCIP data structure */
   );

extern
RETCODE SCIPfree(                       /**< frees SCIP data structures */
   SCIP**           scip                /**< pointer to SCIP data structure */
   );

extern
RETCODE SCIPcreateProb(                 /**< creates empty problem and initializes all solving data structures */
   SCIP*            scip,               /**< SCIP data structure */
   const char*      name                /**< problem name */
   );

extern
RETCODE SCIPfreeProb(                   /**< frees problem and branch-and-bound data structures */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
RETCODE SCIPsolve(                      /**< solves problem */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
RETCODE SCIPfreeSolve(                  /**< frees all solution process data, only original problem is kept */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
RETCODE SCIPcreateVar(                  /**< create problem variable */
   SCIP*            scip,               /**< SCIP data structure */
   VAR**            var,                /**< pointer to variable object */
   const char*      name,               /**< name of column */
   Real             lb,                 /**< lower bound of variable */
   Real             ub,                 /**< upper bound of variable */
   Real             obj,                /**< objective function value */
   VARTYPE          vartype             /**< type of variable */
   );

extern
RETCODE SCIPaddVar(                     /**< adds variable to the problem */
   SCIP*            scip,               /**< SCIP data structure */
   VAR*             var                 /**< variable to add */
   );

extern
RETCODE SCIPcreateRow(                  /**< creates an LP row and captures it */
   SCIP*            scip,               /**< SCIP data structure */
   ROW**            row,                /**< pointer to row */
   const char*      name,               /**< name of row */
   int              len,                /**< number of nonzeros in the row */
   COL**            col,                /**< array with columns of row entries */
   Real*            val,                /**< array with coefficients of row entries */
   Real             rhs,                /**< right hand side of row */
   Real             epsilon,            /**< maximal normed violation of row */
   Bool             equality            /**< is row an equality? otherwise, it is a lower or equal inequality */
   );

extern
RETCODE SCIPcaptureRow(                 /**< increases usage counter of LP row */
   SCIP*            scip,               /**< SCIP data structure */
   ROW*             row                 /**< row to capture */
   );

extern
RETCODE SCIPreleaseRow(                 /**< decreases usage counter of LP row, and frees memory if necessary */
   SCIP*            scip,               /**< SCIP data structure */
   ROW**            row                 /**< pointer to LP row */
   );

extern
RETCODE SCIPincludeConsHdlr(            /**< creates a constraint handler and includes it in SCIP */
   SCIP*            scip,               /**< SCIP data structure */
   const char*      name,               /**< name of constraint handler */
   const char*      desc,               /**< description of constraint handler */
   DECL_CONSINIT((*consinit)),          /**< initialise constraint handler */
   DECL_CONSEXIT((*consexit)),          /**< deinitialise constraint handler */
   DECL_CONSFREE((*consfree)),          /**< frees specific constraint data */
   DECL_CONSTRAN((*constran)),          /**< transforms constraint data into data belonging to the transformed problem */
   DECL_CONSCHCK((*conschck)),          /**< check feasibility of primal solution */
   DECL_CONSPROP((*consprop)),          /**< propagate variable domains */
   CONSHDLRDATA*    conshdlrdata        /**< constraint handler data */
   );

extern
RETCODE SCIPfindConsHdlr(               /**< finds the constraint handler of the given name */
   SCIP*            scip,               /**< SCIP data structure */
   const char*      name,               /**< name of constraint handler */
   CONSHDLR**       conshdlr            /**< pointer for storing the constraint handler (returns NULL, if not found) */
   );

extern
const char* SCIPgetConsHdlrName(        /**< gets name of constraint handler */
   CONSHDLR*        conshdlr            /**< constraint handlert */
   );

extern
RETCODE SCIPincludeNodesel(             /**< creates a node selector and includes it in SCIP */
   SCIP*            scip,               /**< SCIP data structure */
   const char*      name,               /**< name of node selector */
   const char*      desc,               /**< description of node selector */
   DECL_NODESELINIT((*nodeselinit)),    /**< initialise node selector */
   DECL_NODESELEXIT((*nodeselexit)),    /**< deinitialise node selector */
   DECL_NODESELSLCT((*nodeselslct)),    /**< node selection method */
   DECL_NODESELCOMP((*nodeselcomp)),    /**< node comparison method */
   NODESELDATA*     nodeseldata         /**< node selector data */
   );

extern
const char* SCIPgetNodeselName(         /**< gets name of node selector */
   NODESEL*         nodesel             /**< node selector */
   );

extern
RETCODE SCIPcreateCons(                 /**< creates a constraint of the given constraint handler */
   SCIP*            scip,               /**< SCIP data structure */
   CONS**           cons,               /**< pointer to constraint */
   CONSHDLR*        conshdlr,           /**< constraint handler for this constraint */
   CONSDATA*        consdata,           /**< data for this specific constraint */
   Bool             model               /**< is constraint necessary for feasibility? */
   );

extern
RETCODE SCIPaddCons(                    /**< adds constraint to the problem */
   SCIP*            scip,               /**< SCIP data structure */
   CONS*            cons                /**< constraint to add */
   );

extern
RETCODE SCIPaddLocalCons(               /**< adds local constraint to the actual subproblem */
   SCIP*            scip,               /**< SCIP data structure */
   CONS*            cons                /**< constraint to add */
   );

extern
RETCODE SCIPgetChildren(                /**< gets children of actual node */
   SCIP*            scip,               /**< SCIP data structure */
   NODE***          children,           /**< pointer to store children array */
   int*             nchildren           /**< pointer to store number of children */
   );

extern
RETCODE SCIPgetSiblings(                /**< gets siblings of actual node */
   SCIP*            scip,               /**< SCIP data structure */
   NODE***          siblings,           /**< pointer to store siblings array */
   int*             nsiblings           /**< pointer to store number of siblings */
   );

extern
RETCODE SCIPgetBestLeaf(                /**< gets the best leaf from the node queue */
   SCIP*            scip,               /**< SCIP data structure */
   NODE**           bestleaf            /**< pointer to store best leaf */
   );

extern
RETCODE SCIPgetBestNode(                /**< gets the best node from the tree (child, sibling, or leaf) */
   SCIP*            scip,               /**< SCIP data structure */
   NODE**           bestnode            /**< pointer to store best leaf */
   );

extern
VERBLEVEL SCIPverbLevel(                /**< gets verbosity level for message output */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
RETCODE SCIPsetVerbLevel(               /**< sets verbosity level for message output */
   SCIP*            scip,               /**< SCIP data structure */
   VERBLEVEL        verblevel           /**< verbosity level for message output */
   );

extern
STAGE SCIPstage(                        /**< returns current stage of SCIP */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
MEMHDR* SCIPmemhdr(                     /**< returns block memory to use at the current time */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
Real SCIPinfinity(                      /**< returns value treated as infinity */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
Real SCIPepsilon(                       /**< returns value treated as zero */
   SCIP*            scip                /**< SCIP data structure */
   );

extern
Bool SCIPisEQ(                          /**< checks, if values are in range of epsZero */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val1,               /**< first value to be compared */
   Real             val2                /**< second value to be compared */
   );

extern
Bool SCIPisL(                           /**< checks, if val1 is (more than epsZero) lower than val2 */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val1,               /**< first value to be compared */
   Real             val2                /**< second value to be compared */
   );

extern
Bool SCIPisLE(                          /**< checks, if val1 is not (more than epsZero) greater than val2 */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val1,               /**< first value to be compared */
   Real             val2                /**< second value to be compared */
   );

extern
Bool SCIPisG(                           /**< checks, if val1 is (more than epsZero) greater than val2 */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val1,               /**< first value to be compared */
   Real             val2                /**< second value to be compared */
   );

extern
Bool SCIPisGE(                          /**< checks, if val1 is not (more than epsZero) lower than val2 */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val1,               /**< first value to be compared */
   Real             val2                /**< second value to be compared */
   );

extern
Bool SCIPisInfinity(                    /**< checks, if value is infinite */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val                 /**< value to be compared against infinity */
   );

extern
Bool SCIPisZero(                        /**< checks, if value is in range epsZero of 0.0 */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val                 /**< value to be compared against zero */
   );

extern
Bool SCIPisPos(                         /**< checks, if value is greater than epsZero */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val                 /**< value to be compared against zero */
   );

extern
Bool SCIPisNeg(                         /**< checks, if value is lower than -epsZero */
   SCIP*            scip,               /**< SCIP data structure */
   Real             val                 /**< value to be compared against zero */
   );


/*
 * debug methods
 */

#ifndef NDEBUG

extern
void SCIPdebugMemory(                   /**< prints output about used memory */
   SCIP*            scip                /**< SCIP data structure */
   );

#endif


#endif
