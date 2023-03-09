#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
  
#define N 5
#define MISLI 0
#define GLADAN 1
#define JEDE 2
#define LIJEVO (ph_num+4)%N
#define DESNO (ph_num+1)%N
  
sem_t mutex;
sem_t S[N]; 

  
void *filozof(void *num);
void uzima_zlicu(int);
void stavlja_zlicu(int);
void test(int);
  
int state[N]; 
int phil_num[N]={0,1,2,3,4};
  
int main()
{
    int i;
    pthread_t thread_id[N];



    sem_init(&mutex,0,1);
    for(i=0;i<N;i++) 
        sem_init(&S[i],0,0);
    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,filozof,&phil_num[i]);
        printf("Filozof %d misli\n",i+1);
    }
    for(i=0;i<N;i++)
        pthread_join(thread_id[i],NULL); 
}
  
void *filozof(void *num)
{
    while(1)
    {
        int *i = num;
        sleep(1);
        uzima_zlicu(*i); 
        sleep(0);
        stavlja_zlicu(*i); 
    }
}
  
void uzima_zlicu(int ph_num) 
{
    sem_wait(&mutex); 
    state[ph_num] = GLADAN; 
    printf("Filozof %d je gladan\n",ph_num+1);
    test(ph_num); 
    sem_post(&mutex); 
    sem_wait(&S[ph_num]); 
    sleep(1);
}
  
void test(int ph_num) 
{
    if (state[ph_num] == GLADAN && state[LIJEVO] != JEDE && state[DESNO] != JEDE)
    {
        state[ph_num] = JEDE;
        sleep(2);
        printf("Filozof %d uzima vilicu %d i %d\n",ph_num+1,LIJEVO+1,ph_num+1);
        printf("Filozof %d jede\n",ph_num+1);
        sem_post(&S[ph_num]);
    }
}
  
void stavlja_zlicu(int ph_num)
{
    sem_wait(&mutex); 
    state[ph_num] = MISLI; 
    printf("Filozof %d stavlja vilicu %d i %d na stol\n",ph_num+1,LIJEVO+1,ph_num+1);
    printf("Filozof %d misli\n",ph_num+1);
    test(LIJEVO); 
    test(DESNO);
    sem_post(&mutex);
}
