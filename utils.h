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

