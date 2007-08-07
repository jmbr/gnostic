ast.o: ast.c config.h ast-priv.h ast.h xmemory.h
ast-itor.o: ast-itor.c config.h ast-priv.h ast.h stack.h xmemory.h
env.o: env.c config.h logger.h
gnostic.o: gnostic.c config.h env.h logger.h taskset.h task.h ast.h \
  vars.h version.h version.h
grammar.o: grammar.c config.h tasklist.h task.h ast.h taskset.h vars.h \
  logger.h xmemory.h
hashtab.o: hashtab.c config.h hashtab.h xmemory.h
logger.o: logger.c config.h logger.h
scanner.o: scanner.c config.h grammar.h stack.h logger.h xmemory.h
stack.o: stack.c config.h stack.h xmemory.h
task.o: task.c config.h task.h ast.h logger.h xmemory.h
task-exec.o: task-exec.c config.h task.h ast.h env.h logger.h
tasklist.o: tasklist.c config.h tasklist.h task.h ast.h xmemory.h
taskset.o: taskset.c config.h taskset-priv.h task.h ast.h vars.h \
  taskset.h tasklist.h hashtab.h logger.h xmemory.h
taskset-parser.o: taskset-parser.c config.h taskset-priv.h task.h ast.h \
  vars.h taskset.h tasklist.h hashtab.h grammar.h logger.h xmemory.h
test-ast.o: test-ast.c config.h ast.h
test-grammar.o: test-grammar.c config.h grammar.h tasklist.h task.h ast.h
test-hashtab.o: test-hashtab.c config.h hashtab.h xmemory.h
testskeleton.o: testskeleton.c config.h
test-stack.o: test-stack.c config.h stack.h
test-tasklist.o: test-tasklist.c config.h tasklist.h task.h ast.h \
  xmemory.h
vars.o: vars.c config.h vars.h xmemory.h
xmemory.o: xmemory.c config.h
