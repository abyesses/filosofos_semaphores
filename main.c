//
//  main.c
//  semaphore_practice
//
//  Created by Abraham Esses on 3/17/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

sem_t cuchillos;
sem_t tenedores;
sem_t sillas;

void * filosofo_come(void *);
void * filosofo_come(void * id){
    int filosofo = (int) id;
    int tiempo_comiendo;
    sem_wait(&sillas);
    printf("Silla asignada al filósofo %d\n",filosofo);
    sem_wait(&cuchillos);
    printf("Cuchillo asignado al filósofo %d\n",filosofo);
    sem_wait(&tenedores);
    printf("Tenedor asignado al filósofo %d\n",filosofo);
    tiempo_comiendo = rand()%10+1;
    printf("El filósofo %d está comiendo por %d segundos\n",filosofo,tiempo_comiendo);
    sleep(tiempo_comiendo);
    sem_post(&tenedores);
    printf("Tenedor liberado por filósofo %d\n",filosofo);
    sem_post(&cuchillos);
    printf("Cuchillo liberado por filósofo %d\n",filosofo);
    sem_post(&sillas);
    printf("Silla liberado por filósofo %d\n",filosofo);
    printf("El filósofo %d ha salido\n",filosofo);
    pthread_exit(NULL);
}
int main(int argc, const char * argv[]) {
    
    pthread_t * filosofos = (pthread_t *)malloc(100*sizeof(pthread_t));
    sem_init(&cuchillos, 0, 3);
    sem_init(&tenedores, 0, 3);//Inicializando
    sem_init(&sillas, 0, 4);//Inicializando
    signal(SIGALRM, handleAlrm);
    alarm(60);
    int i;
    pthread_t *temp;
    //temp = filosofos;
    for (temp = filosofos,i=0; temp < (filosofos+100); ++temp,++i) {
            pthread_create(temp, NULL, filosofo_come,(void *) i);
    }
    /*while (i <= 100 || !goOn ) {
        ++temp;
        
        printf("I: %d\n",i);
        i++;
    }*/
    for (int j=0; j< i; j++) {
        pthread_join(filosofos[j], NULL);
    }
    
    sem_destroy(&cuchillos);//Destuyendo
    sem_destroy(&tenedores);
    sem_destroy(&sillas);
    
    free(filosofos);
    return 0;
}


