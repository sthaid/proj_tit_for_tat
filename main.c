#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/resource.h>
#include <sys/wait.h>

#include "utils.h"

// prototypes

void doit(char *player1, char *player2);

// -------------------------------------------------------

int main(int argc, char **argv)
{
    struct rlimit rl;

    // use linebuffering for stdout
    setlinebuf(stdout);

    // set resource limti to allow core dumps
    rl.rlim_cur = RLIM_INFINITY;
    rl.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &rl);

    // xxx
    doit("jesus", "jesus");

    return 0;
}

void doit(char *player1, char *player2)
{
    proc_hndl_t *h1, *h2;
    char resp1[100], resp2[100];
    int i;

    printf("PLAYERS:  %s %s\n", player1, player2);

    h1 = proc_run(player1, player1, NULL);  // xxx simplify
    h2 = proc_run(player2, player1, NULL);

    for (i = 1; i <= 5; i++) {
        printf("ROUND %d\n", i);

        proc_printf(h1, "GO\n");
        while (true) {
            proc_gets(h1, resp1, sizeof(resp1));
            if (strcmp(resp1, "ATTACK") == 0 || strcmp(resp1, "DEFEND") == 0) {
                break;
            }
            printf("player1=%s response: %s\n", player1, resp1);
        }

        // xxx don't copy this code
        proc_printf(h2, "GO\n");
        while (true) {
            proc_gets(h2, resp2, sizeof(resp2));
            if (strcmp(resp2, "ATTACK") == 0 || strcmp(resp2, "DEFEND") == 0) {
                break;
            }
            printf("player2=%s response: %s\n", player2, resp2);
        }

        // XXX need to send to pgm the other responses and maintain health

        printf("  %s %s\n", resp1, resp2);
    }

    proc_printf(h1, "TERM\n");
    proc_printf(h2, "TERM\n");

    // xxx timeout on these, or at least debug print if they take a long time
    proc_wait_for_term(h1);
    proc_wait_for_term(h2);

}
