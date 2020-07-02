TARGETS = tft

CC = gcc
OUTPUT_OPTION=-MMD -MP -o $@
CFLAGS = -g -O2 \
         -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-clobbered 

SRC_TFT = main.c utils.c

DEP=$(SRC_TFT:.c=.d)

#
# build rules
#

all: $(TARGETS)

tft: $(SRC_TFT:.c=.o)
	$(CC) -pthread -lreadline -lm -o $@ $(SRC_TFT:.c=.o)

-include $(DEP)

#
# clean rule
#

clean:
	rm -f $(TARGETS) $(DEP) $(DEP:.d=.o)

