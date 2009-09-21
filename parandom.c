
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

static volatile sig_atomic_t g_stop = 0;


static void *
run(void * arg)
{
#define SZ 16
    int rs[SZ];
    while(!g_stop){
        rs[0] = rand();
        rs[1] = rand();
        rs[2] = rand();
        rs[3] = rand();
        rs[4] = rand();
        rs[5] = rand();
        rs[6] = rand();
        rs[7] = rand();
        rs[8] = rand();
        rs[9] = rand();
        rs[10] = rand();
        rs[11] = rand();
        rs[12] = rand();
        rs[13] = rand();
        rs[14] = rand();
        rs[15] = rand();

        fwrite(rs, sizeof(int), SZ, stdout);
    }
}

void
sighandler(int signo)
{
    g_stop = 1;
}

int
main(int argc, char ** argv)
{
    pthread_t * th;
    int threadnum = 16;
    int i;
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sighandler;
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }

    th = malloc(sizeof(pthread_t) * threadnum);
    srand(clock());

    for(i = 0;i < threadnum;i++){
        pthread_create(&th[i], NULL, run, NULL);
    }

    while(!g_stop){
        sleep(1);
        srand(clock());
    }

    for(i = 0;i < threadnum;i++){
        pthread_join(th[i], NULL);
    }
    
    free(th);

    fprintf(stderr, "parandom exit.\n");

    return 0;
}
