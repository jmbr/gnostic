ast.o: ast.c config.h ast.h xmemory.h
err.o: err.c config.h
gnostic.o: gnostic.c config.h taskset.h task.h ast.h version.h err.h
grammar.o: grammar.c config.h tasklist.h task.h ast.h taskset.h err.h \
  xmemory.h
hashtab.o: hashtab.c config.h hashtab.h
scanner.o: scanner.c config.h grammar.h err.h xmemory.h
task.o: task.c config.h task.h ast.h err.h xmemory.h
task-exec.o: task-exec.c config.h task.h ast.h err.h
tasklist.o: tasklist.c config.h tasklist.h task.h ast.h xmemory.h
taskset.o: taskset.c config.h taskset-priv.h task.h ast.h taskset.h \
  tasklist.h hashtab.h err.h xmemory.h
taskset-parser.o: taskset-parser.c config.h taskset-priv.h task.h ast.h \
  taskset.h tasklist.h hashtab.h grammar.h err.h xmemory.h
xmemory.o: xmemory.c config.h
