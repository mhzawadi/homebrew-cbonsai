cbonsai(6)

# NAME

cbonsai - grow bonsai trees in your terminal

# SYNOPSIS

cbonsai [OPTION]...

# DESCRIPTION

cbonsai is a beautifully random bonsai tree generator.

# OPTIONS

*-l*, *--live*
	live mode: show each step of growth

*-t*, *--time*=_TIME_
	in live mode, wait TIME secs between steps of growth (must be larger than 0) [default: 0.03]

*-i*, *--infinite*
	infinite mode: keep growing trees

*-w*, *--wait*=_TIME_
	in infinite mode, wait TIME between each tree generation [default: 4.00]

*-S*, *--screensaver*
	screensaver mode; equivalent to -liWC and quit on any keypress

*-m*, *--message*=_STR_
	attach message next to the tree

*-b*, *--base*=_INT_
	ascii-art plant base to use, 0 is none

*-c*, *--leaf*=_LIST_
	list of comma-delimited strings randomly chosen for leaves

*-M*, *--multiplier*=_INT_
	branch multiplier; higher -> more branching (0-20) [default: 5]

*-L*, *--life*=_INT_
	life; higher -> more growth (0-200) [default: 32]

*-p*, *--print*
	print tree to terminal when finished

*-s*, *--seed*=_INT_
	seed random number generator

*-W*, *--save*=_FILE_
	save progress to file [default: ~/.cache/cbonsai]

*-C*, *--load*=_FILE_
	load progress from file [default: ~/.cache/cbonsai]

*-v*, *--verbose*
	increase output verbosity

*-h*, *--help*
	show help

# AUTHORS

Maintained by John Allbritten <me@johnallbritten.com>, who is assisted by other
open source contributors. For more information about cbonsai development, see
<https://gitlab.com/jallbrit/cbonsai>.
