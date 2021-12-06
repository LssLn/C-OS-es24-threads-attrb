#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);
char message[]="Hello world"; //variabile globale in comune ai 2 thread
int thread_finished=0;
/* flag usato dal secondo thread per indicare al primo che ha finito. Il primo thread non può fare join perché il secondo è creato detached*/

int main(void){
    int res;
    pthread_t a_thread;
    pthread_attr_t thread_attr;
    res=pthread_attr_init(&thread_attr);
    if (res != 0){
        perror("attribute creation failed");
        exit (EXIT_FAILURE);
    }
    res=pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
    if (res != 0){
        perror("setting detached attribute failed");
        exit (EXIT_FAILURE);
    }
    res=pthread_create(&a_thread,&thread_attr,thread_function,(void*)message);
    if (res != 0){
        perror("thread creation failed");
        exit (EXIT_FAILURE);
    }
    (void)pthread_attr_destroy(&thread_attr);
    /* è buona norma cancellare l’attributo del thread dopo l’uso se non serve*/
    while (!thread_finished){ /* non va in wait, e un’attesa attiva (busy waiting) */
        printf("Waiting for thread to say it's finished...\n");
        sleep(1); /* per dare spazio all’altro thread */
    }
    printf("Other thread finished, bye!\n");
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg){
    printf("thread_function is running. Argument was %s\n", (char *)arg);
    sleep(4);
    printf("Second thread setting finished flag and exiting now\n");
    /* imposta la var. globale a 1 per indicare all’altro thread di aver finito */
    thread_finished=1;
    pthread_exit(NULL);
}
