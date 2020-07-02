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

// -------------------------------------------------------

int main(int argc, char **argv)
{
    proc_hndl_t *h;
    char s[1000];
    int cnt=0;
    char *rc;
    struct rlimit rl;
    sigset_t set;

    // use linebuffering for stdout
    setlinebuf(stdout);

    // set resource limti to allow core dumps
    rl.rlim_cur = RLIM_INFINITY;
    rl.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &rl);

    // block signal SIGPIPE
    sigemptyset(&set);
    sigaddset(&set,SIGPIPE);
    sigprocmask(SIG_BLOCK, &set, NULL);

    if (argc == 0) {
        printf("ERROR pgm name needed\n");
        return 0;
    }

    h = proc_run(argv[1], NULL);

    while (true) {
        printf("WRITING TO PGM\n");
        proc_printf(h, "Hello-%d\n", cnt++);

        printf("READING FROM PGM\n");
        rc = proc_gets(h, s, sizeof(s));
        if (rc == NULL) break;
        printf("  '%s'\n", s);

        sleep(1);
    }

    printf("PARENT fgets ret NULL\n");

    proc_wait_for_term(h);
        
    return 0;
}

