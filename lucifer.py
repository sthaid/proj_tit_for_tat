#!/bin/python

import os
import sys

debug=False
id="idnotset"

#
# function definitions
#

def ERROR(*args, **kwargs):
    print("ERROR-{}:".format(id), *args, file=sys.stderr, flush=True, **kwargs)
    exit()

def DEBUG(*args, **kwargs):
    if debug:
        print("DEBUG-{}:".format(id), id, *args, file=sys.stderr, flush=True, **kwargs)

#
# parse command line
#

if len(sys.argv) != 2:
    ERROR("len(sys.argv) = ", len(sys.argv))

id=sys.argv[1]
if id != "0" and id != "1":
    ERROR("expected 0 or 1 on cmdline")

#
# runtime loop
#

while True:
    cmd = input().split()
    DEBUG("RECEIVED", cmd)

    if len(cmd) == 0:
        ERROR("blank line")

    if cmd[0] == "GO":
        print("ATTACK", flush=True)
    elif cmd[0] == "TERM":
        exit()
    else:
        ERROR(" unexpected cmd", cmd[0])

