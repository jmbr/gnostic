#!/bin/sh


GNOSTIC=../gnostic-bin

$GNOSTIC ./circular.gns
RETVAL=$?

if [ $RETVAL -eq 1 ]; then
    exit 0
fi

exit 1


# vim: ts=4 sw=4 et filetype=sh
