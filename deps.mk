ast.o: ast.c config.h ast.h xalloc.h
debug.o: debug.c config.h
gnostic.o: gnostic.c config.h task.h ast.h task-collection.h version.h \
  debug.h
grammar.o: grammar.c config.h task.h ast.h scanner.h xalloc.h debug.h
graph.o: graph.c config.h graph.h graph-priv.h debug.h xalloc.h
htab.o: htab.c config.h htab.h
scanner.o: scanner.c config.h grammar.h xalloc.h debug.h
task.o: task.c config.h task.h ast.h xalloc.h
task-collection.o: task-collection.c config.h task-collection.h task.h \
  ast.h task-collection-priv.h htab.h graph.h xalloc.h debug.h
task-exec.o: task-exec.c config.h task.h ast.h debug.h
task-parser.o: task-parser.c config.h task.h ast.h graph.h htab.h \
  scanner.h grammar.h xalloc.h debug.h
xalloc.o: xalloc.c config.h
