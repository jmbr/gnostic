##############################################################################
#
# libgnst.sh -- Miscellaneous utility functions for gnostic tasklets.
# $Id$
#
##############################################################################


## Converts an IPv4 address to a directory name.
#
ip2dir()
{
	if [ $# -ne 1 ]; then
		exit 1
	fi

	echo $1 | sed -e 's,\.,/,g'
}


getvar()
{
	if [ $# -ne 1 ]; then
		exit 1
	fi

	[[ -f $root/vars/$1 ]] || exit 1

	cat $root/vars/$1
}

setvar()
{
	if [ $# -ne 2 ]; then
		exit 1
	fi

	mkdirhier $root/vars/
	echo $2 > $root/vars/$1
}


# vim: ts=4 sw=4 noet filetype=sh
