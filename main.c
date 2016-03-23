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
int goOn = 1;
void * filosofo_come(void *);
void handleAlrm(int);
void handleAlrm(int fd){
    printf("El restaurante debe cerrarse en 30 segundos\n");
    goOn = 0;
    sleep(30);
    raise(SIGKILL);
}
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
    
    pthread_t * filosofos = (pthread_t *)malloc(sizeof(pthread_t)*100);
    sem_init(&cuchillos, 0, 3);
    sem_init(&tenedores, 0, 3);//Inicializando
    sem_init(&sillas, 0, 4);//Inicializando
    signal(SIGALRM, handleAlrm);
    alarm(60);
    int i=0;
    pthread_t *temp;
    temp = filosofos;
    
    while (i<=100 || !goOn ) {
        ++temp;
        pthread_create(temp, NULL, filosofo_come,(void *) i);
        i++;
    }
    for (int j=0; j< i; j++) {
        pthread_join(filosofos[j], NULL);
    }
    
    sem_destroy(&cuchillos);//Destuyendo
    sem_destroy(&tenedores);
    sem_destroy(&sillas);
    
    free(filosofos);
    return 0;
}


