/* 
 ***********************************************************************
 *
 *                           Coryright (c)
 *    � Digital Equipment Corporation 1996, 1997. All rights reserved.
 *
 *    Restricted Rights: Use, duplication, or disclosure by the U.S.
 *    Government is subject to restrictions as set forth in subparagraph
 *    (c) (1) (ii) of DFARS 252.227-7013, or in FAR 52.227-19, or in FAR
 *    52.227-14 Alt. III, as applicable.
 *
 *    This software is proprietary to and embodies the confidential
 *    technology of Digital Equipment Corporation and other parties.
 *    Possession, use, or copying of this software and media is authorized
 *    only pursuant to a valid written license from Digital or an
 *    authorized sublicensor.
 *
 ***********************************************************************
 *    File Name:    ph_syl1.c
 *    Author:
 *    Creation Date:
 *
 *    Functionality:
 *    language specific syllabification and output 
 *
 ***********************************************************************
 *
 *  Rev Name    Date        Description
 *  --- -----   ----------- --------------------------------------------
 *  001 MGS     03/25/1996  Merged WIN 95 code to 42c
 *  002	GL		04/21/1997	BATS#360  remove spaces before "#define" or "#if" 
 * 0003 DR		09/30/1997		UK BUILD: added UK STUFF
 */

#ifdef ENGLISH_US
#include "p_us_sy1.c"
#endif
#ifdef ENGLISH_UK
#include "p_uk_sy1.c"
#endif
#ifdef SPANISH
#include "p_sp_sy1.c"
#endif
