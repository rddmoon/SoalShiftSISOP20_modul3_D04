#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#define B 4
#define K 5

int matrix[B][K];
int f[B][K];

struct m {
   int i;
   int j;
};

int tambah(int x){
  int i;
  int result = 1;
  if(x == 1){
    return 1;
  }
  else{
    for(i = 1; i <= x; i++){
      result += i;
    }
    return result;
  }
}

void *calc(void *param) {
  struct m *mtrx = param; // untuk menampung mtrx(nilai elemen matrix)
   int result;
   result = tambah(matrix[mtrx->i][mtrx->j]);
   f[mtrx->i][mtrx->j] = result;
   //Exit thread
   pthread_exit(0);
}

void main(){

  key_t key = ftok("/home/yaniarpe/modul3",'a');
  int i, j;

  int shmid = shmget(key, sizeof(int)*B*K, IPC_CREAT|0666);
  int* value = (int *)shmat(shmid, (void*)0, 0);
  printf("Hasil perkalian matriks di 4a:\n");
  for(i = 0; i < B; i++) {
    for(j = 0; j < K; j++) {
      printf("%d\t", value[i*K+j]);
      matrix[i][j] = value[i*K+j];
    }
    printf("\n");
  }
  for(i = 0; i < B; i++) {
     for(j = 0; j < K; j++) {
        
        struct m *mtrx = (struct m *) malloc(sizeof(struct m));
        mtrx->i = i;
        mtrx->j = j;
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid,&attr,calc,mtrx);
        pthread_join(tid, NULL);
     }
  }
  printf("Penjumlahan elemen matriks:\n");
  for(i = 0; i < B; i++){
    for(j = 0; j < K; j++){
      printf("%d\t", f[i][j]);
    }
    printf("\n");
  }
  shmdt(value);
  shmctl(shmid, IPC_RMID, NULL);
}
