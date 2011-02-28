/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2011 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the ZIB Academic License.         */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma ident "@(#) $Id: sepa_closecuts.h,v 1.1 2011/02/27 18:36:14 bzfpfets Exp $"

/**@file   sepa_closecuts.h
 * @brief  closecuts meta separator
 * @author Marc Pfetsch
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __SCIP_SEPA_CLOSECUTS_H__
#define __SCIP_SEPA_CLOSECUTS_H__


#include "scip/scip.h"

#ifdef __cplusplus
extern "C" {
#endif

/** creates the closecuts separator and includes it in SCIP */
extern
SCIP_RETCODE SCIPincludeSepaClosecuts(
   SCIP*                 scip                /**< SCIP data structure */
   );

#ifdef __cplusplus
}
#endif

#endif