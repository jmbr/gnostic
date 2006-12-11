#!/bin/sh

valgrind gnostic $* 2>&1 | awk '
/malloc\/free:\ [0-9]+/ {
	leaked = $3 - $5;
}

END {
	printf("Number of leaked allocs: %d\n", $3 - $5);
}
'
