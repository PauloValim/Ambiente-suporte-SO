//
//  main.c
//  Exemplos
//
//  Created by Paulo Roberto Oliveira Valim on 23/03/17.
//  Copyright © 2017 Paulo Roberto Oliveira Valim. All rights reserved.
//
//Description
/*
 
 A program in which a parent writes a string to a pipe and the child reads the string. The
 program does not check for I/O errors.
 
*/
/* POSIX threads demonstration program */

/* Warning: this example does not check the status return of all Pthread
 *   library functions, and therefore merits a lowered grade.
 * Also, this example doesn't actually do anything like what a real
 *   program should do, it's just intended to demonstrate various
 *   capabilities.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>    /* required for pthreads */
#include <string.h>     /* for strerror() */

/* On Solaris, compile with   cc -v -mt thread_sample.c -lpthread
 *             or             gcc -Wall -Wextra thread_sample.c -lpthread
 * On Mac OS X, compile with  gcc -Wall -Wextra thread_sample.c
 * On Linux, compile with     gcc -Wall -Wextra thread_sample.c -lpthread
 *
 * Note that all the compiler warnings from GCC are about printf format
 * mismatches.  These are easy to fix, but we didn't feel like taking the
 * time, so the grade should be lowered some more.
 *
 * Run with  a.out
 *           a.out 1
 *           a.out 2
 *           a.out 3
 *           a.out 4
 *           a.out 5
 *           a.out 6
 *           a.out 7
 * The operand selects which method to use to end the threads and process.
 */


/* function prototype for Pthread creation (a start function) */
void * thread_func(void * arg);

/* example of data passed to and from a Pthread start function */
typedef struct thread_data {
    int in;	/* used as an input value */
    int out;	/* used as an output value */
} thread_data_t;


/* global variable shared by all the threads of this process */
int quit = 0;


int main(int argc, char * argv[])
{
    pthread_t tid[2];     /* thread ID, to be set by pthread_create() */
    thread_data_t a[2];
    thread_data_t *b[2] = { NULL, NULL };
    /* &a[i] passed to thread i as argument by pthread_create()
     * b[i] received from thread i as return value by pthread_join()
     */
    int ret;
    
    a[0].in = 2; a[0].out = 0;
    a[1].in = 4; a[1].out = 0;
    
    if (argc > 1) quit = atoi(argv[1]);
    
    pthread_create(&tid[0], NULL, thread_func, (void *)&a[0]);
    pthread_create(&tid[1], NULL, thread_func, (void *)&a[1]);
    
    printf("main: process id %d, thread id = %d\n", getpid(), pthread_self());
    printf("main: tid[0] = %d, tid[1] = %d\n", tid[0], tid[1]);
    printf("main: &a[0] = 0x%08lx, &a[1] = 0x%08lx\n", &a[0], &a[1]);
    printf("main:  b[0] = 0x%08lx,  b[1] = 0x%08lx\n",  b[0],  b[1]);
    printf("main: a[0].in  = %d, a[1].in  = %d\n", a[0].in,  a[1].in);
    printf("main: a[0].out = %d, a[1].out = %d\n", a[0].out, a[1].out);
    
    if (quit == 1) pthread_exit(NULL);
    if (quit == 2) exit(0);
    if (quit == 3) pthread_cancel(tid[1]);
    
    pthread_join(tid[0], (void **)&b[0]);
    
    if ((ret = pthread_join(tid[1], (void **)&b[1])) != 0)
    {
        printf("main: pthread_join(tid[1],...) failed on return, %s\n", strerror(ret));
        printf("main: b[1] will be reset from 0x%08x to 0x%08x\n", b[1], NULL);
        b[1] = NULL;
    }
    else if (b[1] != &a[1])
    {
        printf("main: pthread_join(tid[1],...) failed on address, %s\n", strerror(-(int)b[1]));
        printf("main: b[1] will be reset from 0x%08x to 0x%08x\n", b[1], NULL);
        b[1] = NULL;
    }
    
    printf("\n");
    printf("main: process id %d, thread id = %d\n", getpid(), pthread_self());
    printf("main: tid[0] = %d, tid[1] = %d\n", tid[0], tid[1]);
    printf("main: &a[0] = 0x%08lx, &a[1] = 0x%08lx\n", &a[0], &a[1]);
    printf("main:  b[0] = 0x%08lx,  b[1] = 0x%08lx\n",  b[0],  b[1]);
    printf("main: a[0].in   = %d, a[1].in   = %d\n", a[0].in,  a[1].in);
    printf("main: a[0].out  = %d, a[1].out  = %d\n", a[0].out, a[1].out);
    if (b[0] == NULL && b[1] == NULL) {
        printf("main: b[0] is NULL, b[1] is NULL\n");
    } else if (b[0] == NULL) {
        printf("main: b[0] is NULL, b[1]->in = %d, b[1]->out = %d\n", b[1]->in, b[1]->out);
    } else if (b[1] == NULL) {
        printf("main: b[0]->in = %d, b[0]->out = %d, b[1] is NULL\n", b[0]->in, b[0]->out);
    } else {
        printf("main: b[0]->in  = %d, b[1]->in  = %d\n", b[0]->in,  b[1]->in);
        printf("main: b[0]->out = %d, b[1]->out = %d\n", b[0]->out, b[1]->out);
    }
    
    if (quit == 3 && b[1] != NULL && b[1]->out < 0)
        printf("main: thread %d returned b[1]->out = %d\n", tid[1], b[1]->out);
    
    if (quit == 6) pthread_exit(NULL);
    if (quit == 7) exit(0);
    
    return 0;
}

/* standard thread function format for Pthreads */
void * thread_func(void * arg)
{
    /* shared local variable, one instance among all threads using this function */
    static int count = 0;
    
    /* local variable, one instance per thread using this function */
    int s = 0;
    
    /* pointer to input and output struct */
    thread_data_t *p = (thread_data_t *)arg;
    
    count++; s++;
    
    printf("thread: process id %d, thread id = %d, p->in = %d, count = %d, s = %d\n",
           getpid(), pthread_self(), p->in, count, s);
    
    sleep(p->in);
    
    count++; s++;
    
    printf("thread: process id %d, thread id = %d, p->in = %d, count = %d, s = %d\n",
           getpid(), pthread_self(), p->in, count, s);
    
    if (quit == 4)
    {
        p->out = -(int)pthread_self();
        pthread_exit(arg);
    }
    
    if (quit == 5) exit(0);
    
    p->out = (int)pthread_self();
    return arg;
}