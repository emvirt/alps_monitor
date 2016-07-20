
                         Test examples for SafeG
                         =======================
                           by Daniel Sangorrin
                       daniel.sangorrin@gmail.com

This directory contains test examples for SafeG. They are just small tests
and snippsets of code used during development. Therefore they may be unstable
and buggy. Use with care.

0.- Index
    =====

1.- Basic examples
	1.1 obj_safeg_btask
	1.2 obj_safeg_idea
    1.3 obj_safeg_mini
2.- SafeG extensions examples
	2.1 obj_safeg_dualoscom
	2.2 obj_safeg_intsched
	2.3 obj_safeg_intsched_cyclic
	
-------------------------------------------------------------------------------

1.- Basic examples
    ==============

1.1 obj_safeg_btask
    ---------------

T-ASP: periodic high priority task + BTASK
NT-ASP: main loop writing "nt is running"
NT-Linux: normal execution

	$ make OBJ=obj_safeg_btask clean
	$ make OBJ=obj_safeg_btask NONTRUST=linux 1> /dev/null

1.2 obj_safeg_idea
    --------------

Like obj_safeg_btask but for idea6410.

    $ make OBJ=obj_safeg_idea BOARD=SAFEG_IDEA6410 clean
    $ make OBJ=obj_safeg_idea BOARD=SAFEG_IDEA6410 NONTRUST=asp 1> /dev/null

1.3 obj_safeg_mini
    --------------

Like obj_safeg_btask but for mini6410.

    $ make OBJ=obj_safeg_mini BOARD=SAFEG_MINI6410 clean
    $ make OBJ=obj_safeg_mini BOARD=SAFEG_MINI6410 NONTRUST=asp 1> /dev/null

-------------------------------------------------------------------------------

2.- SafeG extensions examples
    =========================

2.1 obj_safeg_dualoscom
    -------------------

Test for the Inter-OS communications extensions.

2.2 obj_safeg_intsched
    ------------------

Test for the Integrated Scheduling extensions with ASP in Non-Trust and a
python script to unifiy logs and display them on TOPPERS/TLV.

2.3 obj_safeg_intsched_cyclic
    -------------------------

Test for the Integrated Scheduling extensions with a Linux cyclic thread 
in Non-Trust.

