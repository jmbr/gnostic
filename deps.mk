ast.o: ast.c config.h ast.h xalloc.h
debug.o: debug.c config.h
gnostic.o: gnostic.c config.h htab.h task.h ast.h graph.h version.h \
  debug.h xalloc.h
grammar.o: grammar.c config.h task.h ast.h htab.h graph.h scanner.h \
  xalloc.h debug.h
graph.o: graph.c config.h graph.h graph-priv.h debug.h xalloc.h
htab.o: htab.c config.h htab.h
scanner.o: scanner.c config.h grammar.h xalloc.h debug.h
task.o: task.c config.h task.h ast.h htab.h graph.h xalloc.h
task-exec.o: task-exec.c config.h task.h ast.h htab.h graph.h debug.h
task-parser.o: task-parser.c config.h task.h ast.h htab.h graph.h \
  scanner.h grammar.h xalloc.h debug.h
xalloc.o: xalloc.c config.h
