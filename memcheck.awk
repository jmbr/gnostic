#!/bin/awk -f

/malloc\/free:\ [0-9]+/ {
	leaked = $3 - $5;

	if (leaked)
		printf("Number of leaked allocs: %d\n", $3 - $5);
}
