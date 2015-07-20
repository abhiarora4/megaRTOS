/*#include <stdio.h>
#include <stdlib.h>
//#include "kernel.h"
//#include "global.h"
#include <time.h>

#define CLOCKS_PER_MSEC CLOCKS_PER_SEC*1000
#define CURRENT ((float)(clock()-time_init)/CLOCKS_PER_MSEC)

extern

enum return_status{rqTrue, rqFalse, pdTrue, pdFalse};
struct semaphore{
    char s;

    struct waitingSem *list;
};

struct waitingSem{
    struct pcb *listSem;
    struct waitingSem *next;
    float del;
    float time_at_block;
};

struct semaphore * mCreateSem(){
    struct semaphore *s=(struct semaphore *)malloc(sizeof(struct semaphore));

    s->s=1;
    s->list=NULL;

    return s;
}

enum return_status mWaitSem(struct pcb *p, struct semaphore *s, float del){
    if(s->s>0){
        p->status=ready;
        return rqTrue;
    }
    p->status=blocked;
    struct waitingSem *temp=(struct waitingSem *)malloc(sizeof(struct waitingSem));
    temp->listSem=p;
    temp->next=NULL;
    temp->del=del;
    temp->time_at_block=CURRENT;

    if(s->list==NULL){
        s->list=temp;
        goto COMPLETED_THE_LIST;

    }

    struct waitingSem *tempBeforeInsert=NULL;
    struct waitingSem *tempList=s->list;

    while(CURRENT-tempList->time_at_block<del){
        if(tempList->next==NULL){
            tempList->next=temp;
            goto COMPLETED_THE_LIST;
            break;
        }
        else{
            tempBeforeEnd=tempList;
            tempList=tempList->next;
        }
    }

    tempBeforeEnd->next=temp;
    temp->next=tempList;

    COMPLETED_THE_LIST:return rqFalse;
}

void mSignalSem(struct semaphore *s){
    s->s=1;
    struct waitingSem *tempTopSem=s->list;
    s->list->listSem->status=ready;

}
*/
