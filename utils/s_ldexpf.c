/* @(#)s_ldexpf.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#include "fdlibm.h"
#include <errno.h>

#ifdef __STDC__
	float ldexpf(float value, int exp)
#else
	float ldexpf(value, exp)
	float value; int exp;
#endif
{
	if(!finitef(value)||value==0.0) return value;
	value = scalbnf(value,exp);
	if(!finitef(value)||value==0.0) errno = ERANGE;
	return value;
}
