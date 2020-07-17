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

// XXX TODO
// - update comments in jesus and lucifer
// - timeout checking
// - comments
// - nicer output format, maybe curses
// - some python scirpts
// - make list of algorithms:  
//       jesus
//       lucifer
//       mrs (massive retalitory strike),
//       tft
//       gtft
//       tester

//
// defines
//

//
// variables
//

//
// prototypes
//

void simulate_conflict(char **player, int *health);

// -------------------------------------------------------

int main(int argc, char **argv)
{
    struct rlimit rl;
    char        **players_list;
    int           max_players_list, i, j;

    // init:
    // - enable debug prints,
    // - use linebuffering for stdout
    // - set resource limti to allow core dumps
    debug = 0;
    setlinebuf(stdout);
    rl.rlim_cur = RLIM_INFINITY;
    rl.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &rl);

    // get list of players from argv
    players_list = &argv[1];
    max_players_list = argc-1;

    // must have at leat 1 player
    if (max_players_list == 0) {
        ERROR("usage: tft <player> <player> ...\n");
        return 1;
    }

    // xxx comment
    for (i = 0; i < max_players_list; i++) {
        int sum_health = 0;
        printf("%s ...\n", players_list[i]);
        for (j = 0; j < max_players_list; j++) {
            char *player[2];
            int health[2];
            player[0] = players_list[i];
            player[1] = players_list[j];
            simulate_conflict(player, health);
            printf("%12s %5d %12s %5d\n", player[0], health[0], player[1], health[1]);
            sum_health += health[0];
        }
        printf("         ---------\n");
        printf("%12s %5d\n", "AVG", sum_health/max_players_list);
        printf("\n");
    }

    return 0;
}

void simulate_conflict(char **player, int *health)
{
    proc_hndl_t *h[2];
    char         response[2][100];
    char         prior_response[2][100];
    int          round, idx;

    #define PLAYERS_ACTIONS_ARE(r0,r1) (strcmp(response[0], (r0)) == 0 && \
                                        strcmp(response[1], (r1)) == 0)

    #define MAX_ROUND 100 //XXX

    memset(health,0,2*sizeof(int));
    memset(response,0,sizeof(response));
    strcpy(prior_response[0], "BE_PASSIVE");
    strcpy(prior_response[1], "BE_PASSIVE");

    // xxx temp
    DEBUG("simulate %12s %12s\n", player[0], player[1]);

    // xxx how to know if they both got created
    h[0] = proc_run(player[0], "0", NULL);
    h[1] = proc_run(player[1], "1", NULL);

    // loop xxx explain
    for (round = 1; round <= MAX_ROUND; round++) {
        // xxx
        //xxx vheck ret here too
        for (idx = 0; idx <= 1; idx++) {
            proc_printf(h[idx], "YESTERDAY_YOUR_OPPONENT %s\n", 
                        (strcmp(prior_response[idx^1], "ATTACK") == 0 
                         ? "ATTACKED" : "WAS_PASSIVE"));

            proc_printf(h[idx], "WHAT_WILL_YOU_DO_TODAY?\n");

            if (proc_gets(h[idx], response[idx], sizeof(response[idx])) == NULL) {
                ERROR("no response from %s\n", player[idx]);
                exit(1);
            }
        }

        strcpy(prior_response[0], response[0]);
        strcpy(prior_response[1], response[1]);

        // update player health
        if (PLAYERS_ACTIONS_ARE("BE_PASSIVE", "BE_PASSIVE")) {
            // no change 
        } else if (PLAYERS_ACTIONS_ARE("ATTACK", "ATTACK")) {
            // both lose 5 points
            health[0] += -5;
            health[1] += -5;
        } else if (PLAYERS_ACTIONS_ARE("ATTACK", "BE_PASSIVE")) {
            // attacker gains 1 point and pacifast loses 10 points
            health[0] +=  1;
            health[1] += -10;
        } else if (PLAYERS_ACTIONS_ARE("BE_PASSIVE", "ATTACK")) {
            // attacker gains 1 point and pacifast loses 10 points
            health[0] += -10;
            health[1] +=  1;
        } else {
            ERROR("players: '%s' '%s', responses: '%s' '%s'\n",
                  player[0], player[1], response[0], response[1]);
            exit(1);
        }

        DEBUG("%8d %12d %12d\n", round, health[0], health[1]);
    }

    proc_printf(h[0], "TERM\n");
    proc_printf(h[1], "TERM\n");

    proc_wait_for_term(h[0]);
    proc_wait_for_term(h[1]);
}

