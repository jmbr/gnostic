##############################################################################
#
# gnostic.sh -- Miscellaneous utility functions for gnostic tasks.
#
##############################################################################


# ============================================================================
# Converts an IPv4 address to a directory name.
# ============================================================================

function ipv4_to_dir
{
	[[ $# -eq 1 ]] || return 1

	echo $1 | sed -e 's,\.,/,g'
}

# ============================================================================
# Returns the IPv4 directory for the current address and root dir.
# ============================================================================

function get_ipv4dir
{
	echo "$root/ipv4/`ipv4_to_dir $addr`"
}


function make_ipv4dir
{
	dir=`get_ipv4dir`
	mkdir -p $dir
	[[ $? -eq 0 ]] || exit 1
	echo $dir
}


# ============================================================================
# Variable accessors. Abstract the way task-defined variables are accessed.
# ============================================================================

function getvar
{
	[[ $# -ne 1 ]] || [[ ! -f $root/vars/$1 ]] && exit 1

	cat $root/vars/$1
}

function setvar
{
	key=$1
	shift

	mkdir -p $root/vars/
	echo $* > $root/vars/$key
}


# ============================================================================
# Progress indicator.
# ============================================================================

function show_progress
{
  thingy[0]="/"
  thingy[1]="-"
  thingy[2]="\\"
  thingy[3]="|"

  count=0
  index=0
  while read line; do
    index=`expr $count % 4`
    printf "\r$* ${thingy[$index]}"
    count=`expr $count + 1`
  done
}

# ============================================================================
# Status reporting
# ============================================================================

function show_status
{
	status=$1
	shift
	msg=$*

	if [ $status -eq 0 ]; then
	  printf "\r$msg done.\n"
	else
	  printf "\r$msg FAILED.\n"
	fi
}


function check_prog
{
	program=$1

	if [ ! -x $program ]; then
		echo "$GNOSTIC_CURRENT: Executable $program not found."
		exit 1
	fi
}


# vim: ts=2 sw=2 noet filetype=sh
