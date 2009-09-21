
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>

#include "SFMT.h"

static volatile sig_atomic_t g_stop = 0;


static void *
run(void * arg)
{
#define SZ 16
    uint32_t rs[SZ];
    while(!g_stop){
        rs[0] = gen_rand32();
        rs[1] = gen_rand32();
        rs[2] = gen_rand32();
        rs[3] = gen_rand32();
        rs[4] = gen_rand32();
        rs[5] = gen_rand32();
        rs[6] = gen_rand32();
        rs[7] = gen_rand32();
        rs[8] = gen_rand32();
        rs[9] = gen_rand32();
        rs[10] = gen_rand32();
        rs[11] = gen_rand32();
        rs[12] = gen_rand32();
        rs[13] = gen_rand32();
        rs[14] = gen_rand32();
        rs[15] = gen_rand32();

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
    init_gen_rand(clock());

    for(i = 0;i < threadnum;i++){
        pthread_create(&th[i], NULL, run, NULL);
    }

    for(i = 0;i < threadnum;i++){
        pthread_join(th[i], NULL);
    }
    
    free(th);

    fprintf(stderr, "parandom exit.\n");

    return 0;
}
