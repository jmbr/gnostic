#!/bin/sh

set -e

autoheader
aclocal
autoconf
shtoolize -o shtool version fixperm tarball mkdir install echo
