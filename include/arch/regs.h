/******************************************************************************
 *   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 *
 *   This file is part of Real VMX.
 *   Copyright (C) 2013 Surplus Users Ham Society
 *
 *   Real VMX is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Real VMX is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Real VMX.  If not, see <http://www.gnu.org/licenses/>.
 */

/* regs.h - Architecture specific CPU regs */

#ifndef _regs_h_
#define _regs_h_

#include <types/vmxCpu.h>

#if	 CPU_FAMILY==I386
#include <arch/i386/regsI386.h>
#endif

#ifndef _ASMLANGUAGE

#ifdef __cplusplus
extern "C" {
#endif

typedef struct regindex
{
    char *regName;              /* Register name */
    int   regOffset;            /* Register offset */
} REG_INDEX;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ASMLANGUANGE */

#endif /* _regs_h */

