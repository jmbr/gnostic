ast.o: ast.c config.h ast.h xalloc.h
debug.o: debug.c config.h
gnostic.o: gnostic.c config.h task.h ast.h taskset.h version.h debug.h
grammar.o: grammar.c config.h task.h ast.h taskset.h xalloc.h debug.h
graph.o: graph.c config.h graph.h graph-priv.h debug.h xalloc.h
hashtab.o: hashtab.c config.h hashtab.h
scanner.o: scanner.c config.h grammar.h xalloc.h debug.h
task.o: task.c config.h task.h ast.h task-priv.h xalloc.h
task-exec.o: task-exec.c config.h task.h ast.h debug.h
taskset.o: taskset.c config.h taskset.h task.h ast.h taskset-priv.h \
  graph.h hashtab.h task-priv.h xalloc.h debug.h
taskset-parser.o: taskset-parser.c config.h taskset.h task.h ast.h \
  taskset-priv.h graph.h hashtab.h grammar.h xalloc.h debug.h
xalloc.o: xalloc.c config.h
