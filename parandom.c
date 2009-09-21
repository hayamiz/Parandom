
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

static void*
run(void* arg){
    int r;
    while(1){
        r = rand();
        fwrite(&r, sizeof(int), 1, stdout);
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
