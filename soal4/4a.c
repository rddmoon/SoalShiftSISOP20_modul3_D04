#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#define B 4
#define BK 2
#define K 5

int a[B][BK] = {{2,4},{1,2},{3,5},{6,1}};
int b[BK][K] = {{3,2,7,2,1},{2,1,4,1,3}};
int c[B][K];

struct m {
   int i;
   int j;
};

void *kali(void *param) {
   struct m *mtrx = param;
   int k, sum = 0;

   for(k = 0; k < BK; k++){
      sum += a[mtrx->i][k] * b[k][mtrx->j];
   }
   c[mtrx->i][mtrx->j] = sum;

   pthread_exit(0);
}


int main(int argc, char *argv[]) {
  key_t key = ftok("/home/yaniarpe/modul3",'a');

  int shmid = shmget(key, sizeof(int)*B*K, IPC_CREAT|0666);
  int* value = (int *)shmat(shmid, (void*)0, 0);
   int i,j;
   printf("Matriks pertama:\n");
   for(i = 0; i < B; i++){
     for(j = 0; j < BK; j++){
       printf("%d\t", a[i][j]);
     }
     printf("\n");
   }
   printf("Matriks kedua:\n");
   for(i = 0; i < BK; i++){
     for(j = 0; j < K; j++){
       printf("%d\t", b[i][j]);
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
         pthread_create(&tid,&attr,kali,mtrx);
         pthread_join(tid, NULL);
      }
   }

   printf("Matriks hasil perkalian:\n");
   for(i = 0; i < B; i++) {
      for(j = 0; j < K; j++) {
         printf("%d\t", c[i][j]);
         value[i*K+j] = c[i][j];
      }
      printf("\n");
   }
   shmdt(value);
   //shmctl(shmid, IPC_RMID, NULL);
}
