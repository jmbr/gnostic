#ifndef GNOSTIC_H
#define GNOSTIC_H
/*
 * \file gnostic.h
 * \brief generic network scanning tool.
 */
/* $Id$ */


#include "task.h"
#include "htab.h"
#include "graph.h"


struct gnostic {
	htab_t symtab;  	/**< Symbol table */
	graph_t depgraph;	/**< Dependency graph */
	struct task *tasks;	/**< Task list */
};


extern struct gnostic *new_gnostic(void);
extern int delete_gnostic(struct gnostic *self);

extern int gnostic_conf_parse(struct gnostic *self, const char *filename);


#endif /* !GNOSTIC_H */
