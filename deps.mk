ast.o: ast.c config.h ast.h xalloc.h
debug.o: debug.c config.h
exec.o: exec.c config.h exec.h task.h ast.h debug.h
gnostic.o: gnostic.c config.h gnostic.h task.h ast.h htab.h graph.h \
  exec.h version.h debug.h xalloc.h
grammar.o: grammar.c config.h task.h ast.h scanner.h xalloc.h debug.h
graph.o: graph.c config.h graph.h graph-priv.h debug.h xalloc.h
htab.o: htab.c config.h htab.h
parser.o: parser.c config.h gnostic.h task.h ast.h htab.h graph.h \
  scanner.h grammar.h xalloc.h debug.h
scanner.o: scanner.c config.h grammar.h xalloc.h debug.h
task.o: task.c config.h task.h ast.h xalloc.h
xalloc.o: xalloc.c config.h
