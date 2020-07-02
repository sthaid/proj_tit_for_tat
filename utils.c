#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <signal.h>
#include <sys/wait.h>
#include <assert.h>

#include "utils.h"

// --------------------  CHILD PROCESS USING FORK & EXEC  ----------------------

proc_hndl_t * proc_run(char *proc, ...)
{
    char *args[100];
    int argc=0;
    int pipe_to_child[2], pipe_from_child[2];
    int pid;
    va_list ap;
    proc_hndl_t *proc_hndl;

    // construct args array
    args[argc++] = proc;
    va_start(ap, proc);
    while (true) {
        args[argc] = va_arg(ap, char*);
        if (args[argc] == NULL) break;
        argc++;
    }
    va_end(ap);

#if 1
    // xxx
    int i;
    for (i = 0; i < argc+1; i++) {
        printf("RUN_PGM PARENT  %d: %s\n", i, args[i]);
    }
#endif

    // create pipes for proc input and output
    // - pipefd[0] is read end, pipefd[1] is write end
    pipe(pipe_to_child);
    pipe(pipe_from_child);

    // fork
    pid = fork();
    if (pid == -1) {
        return NULL;
    }

    // if pid == 0, child is running, else parent
    if (pid == 0) {
        // child ..
        // close unused ends of the pipes
        close(pipe_to_child[1]);
        close(pipe_from_child[0]);

        // attach the 2 pipes to stdin and stdout for the child
        // xxx what about stderr
        dup2(pipe_to_child[0], 0);
        dup2(pipe_from_child[1], 1);

        // execute the program
        execvp(proc, args);
        fprintf(stderr, "ERROR: execvp %s, %s\n", proc, strerror(errno));
        exit(1);        
    } else {
        FILE *fp_to_proc;
        FILE *fp_from_proc;

        // parent ... 
        // close unused ends of the pipes
        close(pipe_to_child[0]);
        close(pipe_from_child[1]);

        // create FPs from the pipe fds
        fp_to_proc = fdopen(pipe_to_child[1], "w");
        fp_from_proc = fdopen(pipe_from_child[0], "r");

        // set fp write to proc line buffered
        setlinebuf(fp_to_proc);

        // return proc_hndl to caller
        proc_hndl = malloc(sizeof(proc_hndl_t));
        proc_hndl->fd_to_proc = pipe_to_child[1];
        proc_hndl->fd_from_proc = pipe_from_child[0];
        proc_hndl->fp_to_proc = fp_to_proc;
        proc_hndl->fp_from_proc = fp_from_proc;
        proc_hndl->pid = pid;

        // return proc_hndl
        return proc_hndl;
    }
}

void proc_wait_for_term(proc_hndl_t *h)
{
    int wstatus;

    waitpid(h->pid, &wstatus, 0);
    printf("WAITPID %d %d\n", WIFEXITED(wstatus), WEXITSTATUS(wstatus));

    fclose(h->fp_to_proc);
    fclose(h->fp_from_proc);
    close(h->fd_to_proc);
    close(h->fd_from_proc);

    free(h);
}

void proc_printf(proc_hndl_t *h, char *fmt, ...)
{
    va_list ap;

    va_start(ap,fmt);
    vfprintf(h->fp_to_proc, fmt, ap);
    va_end(ap);
}

void proc_puts(proc_hndl_t *h, char *s)
{
    fputs(s, h->fp_to_proc);
}

char * proc_gets(proc_hndl_t *h, char *s, int sizeofs)
{
    assert(s && sizeofs > 0);

    s[0] = '\0';

    s = fgets(s, sizeofs, h->fp_from_proc);
    if (s != NULL) {
        int len = strlen(s);
        if (len > 0 && s[len-1] == '\n') {
            s[len-1] = '\0';
        }
    }

    return s;
}

void proc_kill(proc_hndl_t *h)
{
    kill(h->pid, SIGKILL);
}

void proc_abort(proc_hndl_t *h)
{
    kill(h->pid, SIGABRT);
}

