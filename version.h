/*
**  version.h -- Version Information for gnostic (syntax: C/C++)
**  [automatically generated and maintained by GNU shtool]
*/

#ifdef _VERSION_H_AS_HEADER_

#ifndef _VERSION_H_
#define _VERSION_H_

#define VERSION 0x002205

typedef struct {
    const int   v_hex;
    const char *v_short;
    const char *v_long;
    const char *v_tex;
    const char *v_gnu;
    const char *v_web;
    const char *v_sccs;
    const char *v_rcs;
} version_t;

extern version_t version;

#endif /* _VERSION_H_ */

#else /* _VERSION_H_AS_HEADER_ */

#define _VERSION_H_AS_HEADER_
#include "version.h"
#undef  _VERSION_H_AS_HEADER_

version_t version = {
    0x002205,
    "0.2.5",
    "0.2.5 (19-Jul-2004)",
    "This is gnostic, Version 0.2.5 (19-Jul-2004)",
    "gnostic 0.2.5 (19-Jul-2004)",
    "gnostic/0.2.5",
    "@(#)gnostic 0.2.5 (19-Jul-2004)",
    "$Id: gnostic 0.2.5 (19-Jul-2004) $"
};

#endif /* _VERSION_H_AS_HEADER_ */

