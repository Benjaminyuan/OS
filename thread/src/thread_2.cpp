#include<pthread.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<iostream>
#define SELL_THREADS 10
#define TICKETS_RANGE 10
using namespace std;
typedef int MODIFYSTATUS ;
MODIFYSTATUS SUCCESS = 1;
MODIFYSTATUS SOLDOUT = 0;
MODIFYSTATUS OUTOFREMAIN = -1;
void P(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = SEM_UNDO;
    semop(semid, &sem, 1);
}
void V(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = SEM_UNDO;
    semop(semid, &sem, 1);
}
MODIFYSTATUS modifiy_ticket(int op_num,int thread_id){
    static int TICKETS = 1000;
    static int SOLD_TICKETS = 0;
    if(TICKETS - op_num < 0){
        return OUTOFREMAIN;
    }
    if(TICKETS - op_num > 0){
        TICKETS -= op_num;
        SOLD_TICKETS += op_num;
        cout<<"thread_id: "<< thread_id << " buy "<< op_num<< " tickets success!!" <<endl;
        return SUCCESS;
    }
    return SOLDOUT;
}
void *sell_tickets(void * arg){
    int semid = ((int*)arg)[0];
    int thread_id = ((int*)arg)[1];
    cout<<"thread:"<< thread_id<<"start" <<endl;
    while (1)
    {
        srand((unsigned int)time(NULL));
        int buy_tickets =rand() % TICKETS_RANGE + 1;
        P(semid,0);
        MODIFYSTATUS status =  modifiy_ticket(buy_tickets,thread_id);
        if(status == SOLDOUT){
            break;
        }
        V(semid,0);
        sleep(1);
    }
    pthread_exit(NULL);
}
int main(){
    int semid = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    // 写锁
    semctl(semid,0,SETVAL,1);
    pthread_t sell_threads[SELL_THREADS];
    for(int i=0;i<SELL_THREADS;i++){
        int params[2] ;
        params[0] = semid;
        params[1] = i+1;
        pthread_create(&sell_threads[i],NULL,sell_tickets,(void *)&params);
    }
    for(int i=0;i<SELL_THREADS;i++){
        pthread_join(sell_threads[i],NULL);
    }

}