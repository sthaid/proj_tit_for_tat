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

void simulate_conflict(char *player1, char *player2);

// -------------------------------------------------------

int main(int argc, char **argv)
{
    struct rlimit rl;
    char        **players;
    int           max_players, i, j;

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
    players = &argv[1];
    max_players = argc-1;

    // must have at leat 1 player
    if (max_players == 0) {
        ERROR("usage: tft <player> <player> ...\n");
        return 1;
    }

    // xxx comment
    for (i = 0; i < max_players; i++) {
        printf("%s ...\n", players[i]);
        for (j = 0; j < max_players; j++) {
            simulate_conflict(players[i], players[j]);
        }
        printf("\n");
    }

    return 0;
}

void simulate_conflict(char *player0_arg, char *player1_arg)
{
    proc_hndl_t *h[2];
    char         response[2][100];
    char         prior_response[2][100];
    int          round, idx, health[2];
    char        *player[2];

    #define PLAYERS_ACTIONS_ARE(r0,r1) (strcmp(response[0], (r0)) == 0 && \
                                        strcmp(response[1], (r1)) == 0)

    #define MAX_ROUND 100

    memset(health,0,sizeof(health));
    memset(response,0,sizeof(response));
    memset(prior_response,0,sizeof(prior_response));
    player[0] = player0_arg;
    player[1] = player1_arg;

    // xxx temp
    DEBUG("simulate %12s %12s\n", player[0], player[1]);

    // xxx how to know if they both got created
    h[0] = proc_run(player[0], "0", NULL);
    h[1] = proc_run(player[1], "1", NULL);

    // loop xxx explain
    for (round = 1; round <= MAX_ROUND; round++) {
        // xxx
        for (idx = 0; idx <= 1; idx++) {
            proc_printf(h[idx], "GO %s\n", prior_response[idx^1]);
//xxx vheck ret here too
            if (proc_gets(h[idx], response[idx], sizeof(response[idx])) == NULL) {
                ERROR("no response from %s\n", player[idx]);
                exit(1);
            }
        }

        strcpy(prior_response[0], response[0]);
        strcpy(prior_response[1], response[1]);

        // update player health
        if (PLAYERS_ACTIONS_ARE("PASSIVE", "PASSIVE")) {
            // no change 
        } else if (PLAYERS_ACTIONS_ARE("ATTACK", "ATTACK")) {
            // both lose 5 points
            health[0] += -5;
            health[1] += -5;
        } else if (PLAYERS_ACTIONS_ARE("ATTACK", "PASSIVE")) {
            // attacker gains 1 point and pacifast loses 10 points
            health[0] +=  1;
            health[1] += -10;
        } else if (PLAYERS_ACTIONS_ARE("PASSIVE", "ATTACK")) {
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

    // xxx more comments
    printf("%12s %5d %12s %5d\n", player[0], health[0], player[1], health[1]);

    proc_printf(h[0], "TERM\n");
    proc_printf(h[1], "TERM\n");

    proc_wait_for_term(h[0]);
    proc_wait_for_term(h[1]);
}

#if 0
        // if both players are PASSIVE
        //   no change in either player's health
        // else if both ATTACK
        //   both players lose 5 health points
        // else if player1 ATTACKs and player2 is PASSIVE
        //   player1 health increases by 1
        //   player2 health decreases by 10
        // else if player2 ATTACKs and player1 is PASSIVE
        //   player2 health increases by 1
        //   player1 health decreases by 10
        // endif


#endif
