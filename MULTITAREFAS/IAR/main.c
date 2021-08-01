#define __USE_GNU 1 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <semaphore.h>

#define N_ITENS 30
int buffer[N_ITENS];

semaforo_t pos_vazia;
semaforo_t pos_ocupada;

int inicio = 0, final = 0;

void* produtor(void *v) {
  int i;

  for (i = 0; i < 3 * N_ITENS; i++) {
    SemaforoAguarda(&pos_vazia);
    printf("Produtor, item = %d.\n", i);     
    final = (final + 1) % N_ITENS;
    buffer[final] = i;
    SemaforoLibera(&pos_ocupada);
    sleep(random() % 3);  /* Permite que a outra thread execute */
  }
  return NULL;
}

void* consumidor(void *v) {
  int i;

  for (i = 0; i < 3 * N_ITENS; i++) {
    SemaforoAguarda(&pos_ocupada);
    inicio = (inicio + 1) % N_ITENS;
    printf("Consumidor, item = %d.\n", buffer[inicio]);
    SemaforoLibera(&pos_vazia);
    sleep(random() % 3);  /* Permite que a outra thread execute */  
  }
  return NULL;
}

int main() {

  pthread_t thr_produtor, thr_consumidor;

  sem_init(&pos_vazia, 0, N_ITENS);
  sem_init(&pos_ocupada, 0, 0);
  
  pthread_create(&thr_produtor, NULL, produtor, NULL);
  pthread_create(&thr_consumidor, NULL, consumidor, NULL);

  pthread_join(thr_produtor, NULL); 
  pthread_join(thr_consumidor, NULL);

  sem_destroy(&pos_vazia);
  sem_destroy(&pos_ocupada);
  
  return 0;
}
