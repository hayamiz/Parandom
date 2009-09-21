
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

static void*
run(void* arg){
#define SZ 16
    int rs[SZ];
    while(1){
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

int
main(int argc, char** argv){
    pthread_t* th;
    int threadnum = 16;
    int i;

    th = malloc(sizeof(pthread_t) * threadnum);
    srand(clock());

    for(i = 0;i < threadnum;i++){
        pthread_create(&th[i], NULL, run, NULL);
    }

    for(i = 0;i < threadnum;i++){
        pthread_join(th[i], NULL);
    }
    
    free(th);

    return 0;
}
