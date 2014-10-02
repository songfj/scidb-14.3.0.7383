/*
**
* BEGIN_COPYRIGHT
*
* This file is part of SciDB.
* Copyright (C) 2008-2013 SciDB, Inc.
*
* SciDB is free software: you can redistribute it and/or modify
* it under the terms of the AFFERO GNU General Public License as published by
* the Free Software Foundation.
*
* SciDB is distributed "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
* INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
* NON-INFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE. See
* the AFFERO GNU General Public License for the complete license terms.
*
* You should have received a copy of the AFFERO GNU General Public License
* along with SciDB.  If not, see <http://www.gnu.org/licenses/agpl-3.0.html>
*
* END_COPYRIGHT
*/

#ifndef BSDIFF_H_
#define BSDIFF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sys/types.h>

int bsdiff_nocompress(u_char*, off_t, u_char*, off_t, u_char*, off_t, off_t*);
int bspatch_nocompress(u_char*, off_t, u_char*, off_t, u_char**, off_t*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BSDIFF_H_ */
