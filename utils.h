// ----------------------------------------

#define INFO(fmt, args...) \
    do { \
        logmsg("INFO", __func__, fmt, ## args); \
    } while (0)
#define WARN(fmt, args...) \
    do { \
        logmsg("WARN", __func__, fmt, ## args); \
    } while (0)
#define ERROR(fmt, args...) \
    do { \
        logmsg("ERROR", __func__, fmt, ## args); \
    } while (0)

#define DEBUG(fmt, args...) \
    do { \
        if (debug) { \
            logmsg("DEBUG", __func__, fmt, ## args); \
        } \
    } while (0)

extern int debug;

void logmsg(char * lvl, const char * func, char * fmt, ...) __attribute__ ((format (printf, 3, 4)));

// ----------------------------------------

typedef struct {
    int fd_to_proc;
    int fd_from_proc;
    FILE *fp_to_proc;
    FILE *fp_from_proc;
    int pid;
} proc_hndl_t;

proc_hndl_t * proc_run(char *proc, ...);
void proc_wait_for_term(proc_hndl_t *h);
void proc_printf(proc_hndl_t *h, char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
void proc_puts(proc_hndl_t *h, char *s);
char * proc_gets(proc_hndl_t *h, char *s, int sizeofs);
void proc_kill(proc_hndl_t *h);
void proc_abort(proc_hndl_t *h);

