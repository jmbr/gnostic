#!/bin/sh

GNOSTIC=./gnostic

TEST_FILES=`ls -1 ./testdata/*.gns`

STATUS=0

check()
{
    $GNOSTIC $1
    STATUS=$[$STATUS + $?]
}

for f in $TEST_FILES; do
    check $f
done

if [ $STATUS -ne `echo $TEST_FILES | wc -w` ]; then
    exit 1
fi

exit 0


# vim: ts=4 sw=4 et filetype=sh
