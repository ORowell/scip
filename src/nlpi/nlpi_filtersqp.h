/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2021 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the ZIB Academic License.         */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SCIP; see the file COPYING. If not visit scipopt.org.         */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file    nlpi_filtersqp.h
 * @brief   filterSQP NLP interface
 * @ingroup NLPIS
 * @author  Stefan Vigerske
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __SCIP_NLPI_FILTERSQP_H__
#define __SCIP_NLPI_FILTERSQP_H__

#include "nlpi/type_nlpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/** create solver interface for filterSQP solver */
SCIP_RETCODE SCIPcreateNlpSolverFilterSQP(
   BMS_BLKMEM*           blkmem,             /**< block memory data structure */
   SCIP_NLPI**           nlpi                /**< pointer to buffer for nlpi address */
   );

/** gets string that identifies filterSQP */
const char* SCIPgetSolverNameFilterSQP(void);

/** gets string that describes filterSQP */
const char* SCIPgetSolverDescFilterSQP(void);

/** returns whether filterSQP is available, i.e., whether it has been linked in */
SCIP_Bool SCIPisFilterSQPAvailableFilterSQP(void);

#ifdef __cplusplus
}
#endif

#endif /* __SCIP_NLPI_FILTERSQP_H__ */
