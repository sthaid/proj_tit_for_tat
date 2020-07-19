TARGETS = ctlr lucifer

CC = gcc
OUTPUT_OPTION=-MMD -MP -o $@
CFLAGS = -g -O2 \
         -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-clobbered 

SRC_CTLR    = ctlr.c utils.c
SRC_LUCIFER = lucifer.c

DEP=$(SRC_CTLR:.c=.d) \
    $(SRC_LUCIFER:.c=.d)

#
# build rules
#

all: $(TARGETS)

ctlr: $(SRC_CTLR:.c=.o)
	$(CC) -pthread -lreadline -lm -o $@ $(SRC_CTLR:.c=.o)

lucifer: $(SRC_LUCIFER:.c=.o)
	$(CC) -o $@ $(SRC_LUCIFER:.c=.o)

-include $(DEP)

#
# clean rule
#

clean:
	rm -f $(TARGETS) $(DEP) $(DEP:.d=.o)

