#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//
// defines
//

#define ERROR(fmt, args...) \
    do { \
        fprintf(stderr, "%s-%s: ERROR: ", progname, idstr); \
        fprintf(stderr, fmt, ## args); \
        exit(1); \
    } while (0)

#define DEBUG(fmt, args...) \
    do { \
        if (debug) { \
            fprintf(stderr, "%s-%s: DEBUG: ", progname, idstr); \
            fprintf(stderr, fmt, ## args); \
        } \
    } while (0)

//
// variables
//

int   debug = 0;   // set to 1 to enable debug prints
char *progname;
char *idstr;

//
// prototypes
//

static void remove_newline(char *s);

// -----------------  LUCIFER  -----------------------------------------------

int main(int argc, char **argv)
{
    char *cmd, *arg, input[100];
    bool yesterday_opponent_attacked;

    // must use line buffered stdout, 
    // because stdout is a pipe to the ctlr program
    setlinebuf(stdout);

    // argc must be 2
    if (argc != 2) {
        ERROR("argc=%d, must be 2\n", argc);
    }

    // get progname and idstr from command line args
    progname = argv[0];
    idstr = argv[1];
    if (strcmp(idstr, "0") && strcmp(idstr, "1")) {
        ERROR("argv[1] must be either '1' or '2'\n");
    }

    while (fgets(input, sizeof(input), stdin) != NULL) {
        // remove trailing newline char, this is not usually needed
        remove_newline(input);
        DEBUG("RECEIVED %s\n", input);

        // use strtok to splt input to cmd and arg
        cmd = strtok(input, " \n");
        arg = strtok(NULL, " \n");

        // input must have a cmd
        if (cmd == NULL) {
            ERROR("blank line\n");
        }

        // process input cmd
        if (strcmp(cmd, "YESTERDAY_YOUR_OPPONENT") == 0) {
            yesterday_opponent_attacked = (strcmp(arg, "ATTACK") == 0);
            DEBUG("  yesterday_opponent_attacked = %d\n", yesterday_opponent_attacked);
        } else if (strcmp(cmd, "WHAT_WILL_YOU_DO_TODAY?") == 0) {
            printf("ATTACK\n");
        } else if (strcmp(cmd, "TERM") == 0) {
            exit(0);
        } else {
            ERROR("unexpected cmd '%s'\n", cmd);
        }
    }
}

static void remove_newline(char *s)
{
    int len = strlen(s);

    if (len > 0 && s[len-1] == '\n') {
        s[len-1] = '\0';
    }
}
