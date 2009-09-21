
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include "SFMT.h"

static volatile sig_atomic_t g_stop = 0;


static void *
run(void * arg)
{
#define SZ 624 * 128
    uint32_t* rs;
    if (posix_memalign(&rs, 16, sizeof(uint32_t) * SZ) != 0){
        perror("posix_memalign");
        exit(errno);
    }
    while(!g_stop){
        fill_array32(rs, SZ);

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
    int threadnum = 1;
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

    while(!g_stop){
        sleep(1);
        init_gen_rand(clock());
    }

    for(i = 0;i < threadnum;i++){
        pthread_join(th[i], NULL);
    }
    
    free(th);

    fprintf(stderr, "parandom exit.\n");

    return 0;
}
