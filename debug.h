#ifndef GNOSTIC_DEBUG_H
#define GNOSTIC_DEBUG_H			1
/*
 * debug.h -- Misc. debugging features.
 * $Id: debug.h,v 1.1.1.1 2001/12/07 20:56:20 rwx Exp $
 */


extern void dprintf(const char *format, ...);
extern void eprintf(const char *format, ...);
extern void fatal_error(const char *format, ...);


#endif /* GNOSTIC_DEBUG_H */
