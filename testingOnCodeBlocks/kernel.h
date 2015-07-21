#ifndef _kernel_h_
#define _kernel_h_

#include<time.h>
#include "global.h"
#define CURRENT ((float)(clock()-time_init)/CLOCKS_PER_MSEC)


enum status {init, ready, running, waiting, blocked, suspended, finished};
enum priority {HIGH, LOW, MEDIUM};

struct pcb{
void (*pointer_to_function)();
enum status status;
enum priority level;
int pid;
};

struct wait_list{
    struct pcb *pcb_wait;
    struct wait_list *next;
    float time_at_block;
    float del;
    float time_at_resume;
};

struct suspend_list{
    struct pcb *pcb_suspend;
    struct suspend_list *next;
    float time_atMost_resume;
    float time_at_suspend;
};


void (*func)();

void scheduler(struct wait_list **, struct suspend_list **);
void * mTaskCreate(void (*process)(), enum priority);

void wait(struct wait_list **, float );
void suspend(struct suspend_list **, float );

void delay(unsigned int);

void RTOS_debugger();

void scheduler(struct wait_list **t, struct suspend_list **s){
while(1){



//suspend list
/*
struct suspend_list *endS=*s;
struct suspend_list *Second_last;

    while(endS->next!=NULL)
    {
      Second_last=endS;
      endS=endS->next;
    }

struct suspend_list *pointBeforeInst=*s;
struct suspend_list *pointOfInst;
pointOfInst=NULL;

while(pointBeforeInst->next!=NULL)
{

if(endS->time_atMost_resume<pointBeforeInst->time_atMost_resume){
    endS->next=pointBeforeInst;
    if(pointOfInst!=NULL)
        pointOfInst->next=endS;
    else
        *s=endS;
    Second_last->next=NULL;
    break;
}

pointOfInst=pointBeforeInst;
pointBeforeInst=pointBeforeInst->next;

}


delay(100);

if(CURRENT>((*s)->time_atMost_resume)){
    func=(*s)->pcb_suspend->pointer_to_function;
    current_pcb=(*s)->pcb_suspend;
    func();
}

else */

delay(100);
if((*t)!=NULL&&CURRENT>=(*t)->time_at_resume){
//test addition
struct wait_list *tempy=*t;
    while(tempy->next!=NULL)
    {
      printf("%4.2f of %d->", tempy->time_at_resume/1000, tempy->pcb_wait->pid);
      //printf("%d ->", temp->pid);
      tempy=tempy->next;
    }
printf("%4.2f of %d \t", tempy->time_at_resume/1000, tempy->pcb_wait->pid);
//printf("%d \t", temp->pid);
//ends


mSchedulerWait(t);


func();
}

else if((*s)!=NULL){

    mSchedulerSuspend(s);
    func();
}

}
}

void mSchedulerWait(struct wait_list **t){
    func=(*t)->pcb_wait->pointer_to_function;
    current_pcb=(*t)->pcb_wait;
    (*t)->pcb_wait->status=running;

    current_wait_list_pcb=(*t);
    if((*t)->next!=NULL)
        (*t)=(*t)->next;
    else
        (*t)=NULL;
    current_wait_list_pcb->next=NULL;

    current_suspend_list_pcb=NULL;
}



void mSchedulerSuspend(struct suspend_list **s){

    func=(*s)->pcb_suspend->pointer_to_function;
    current_pcb=(*s)->pcb_suspend;

    (*s)->pcb_suspend->status=running;

    current_suspend_list_pcb=(*s);
    if((*s)->next!=NULL)
        (*s)=(*s)->next;
    else
        (*s)=NULL;
    current_suspend_list_pcb->next=NULL;

    current_wait_list_pcb=NULL;

}



void * mTaskCreate(void (*process)(), enum priority lev){

    static int i;

    process_control_block[i].pointer_to_function=process;
    process_control_block[i].status=ready;
    void *ret;

    struct suspend_list *temp=(struct suspend_list*)malloc(sizeof(struct suspend_list));
    temp->pcb_suspend=&(process_control_block[i]);
    temp->time_at_suspend=((float)(clock()-time_init))/CLOCKS_PER_SEC;
    temp->time_atMost_resume=temp->time_at_suspend;
    temp->pcb_suspend->pid=i;
    if(topSuspendList==NULL){
        topSuspendList=temp;
        temp->next=NULL;
    }
    else{
        temp->next=topSuspendList;
        topSuspendList=temp;
    }

    ret=topSuspendList->pcb_suspend;

    i++;
    return ret;
}


void wait(struct wait_list **t, float d){

    if(current_wait_list_pcb==NULL){
        current_wait_list_pcb=(struct wait_list *)malloc(sizeof(struct wait_list));
        current_wait_list_pcb->pcb_wait=current_pcb;
    }
    current_wait_list_pcb->time_at_block=((float)(clock() - time_init)/CLOCKS_PER_MSEC);
    current_wait_list_pcb->del=d;
    current_wait_list_pcb->time_at_resume=current_wait_list_pcb->time_at_block+d;
    current_wait_list_pcb->pcb_wait->status=waiting;


    //wait list
    struct wait_list *tempPointOfInsert=*t;
    struct wait_list *tempBeforeInsert;
    tempBeforeInsert=NULL;
    if(tempPointOfInsert==NULL){
        *t=current_wait_list_pcb;
        tempPointOfInsert=*t;
        goto DONE;
    }

    while(tempPointOfInsert!=NULL)
    {

    if(current_wait_list_pcb->time_at_resume<=tempPointOfInsert->time_at_resume){
    //tempLastWaitList->next=tempPointOfInsert;
    if(tempBeforeInsert!=NULL){
        tempBeforeInsert->next=current_wait_list_pcb;
        current_wait_list_pcb->next=tempPointOfInsert;
    }
    else{
        *t=current_wait_list_pcb;
        (*t)->next=tempPointOfInsert;
        //tempSecondLastWaitList->next=NULL;
    }
    goto DONE;
}

    tempBeforeInsert=tempPointOfInsert;
    tempPointOfInsert=tempPointOfInsert->next;
}
    if(tempPointOfInsert!=NULL)
        tempPointOfInsert->next=current_wait_list_pcb;
    else
        tempBeforeInsert->next=current_wait_list_pcb;


    DONE:return;
}

void suspend(struct suspend_list **s, float del){

    if(current_suspend_list_pcb==NULL){
        current_suspend_list_pcb=(struct suspend_list *)malloc(sizeof(struct suspend_list));
        current_suspend_list_pcb->pcb_suspend=current_pcb;
    }
    current_suspend_list_pcb->time_at_suspend=((float)(clock() - time_init)/CLOCKS_PER_MSEC);
    current_suspend_list_pcb->time_atMost_resume=current_suspend_list_pcb->time_at_suspend+del;
    current_suspend_list_pcb->pcb_suspend->status=waiting;


    //wait list
    struct suspend_list *tempPointOfInsert=*s;
    struct suspend_list *tempBeforeInsert;
    tempBeforeInsert=NULL;
    if(tempPointOfInsert==NULL){
        tempPointOfInsert=current_suspend_list_pcb;
        goto DONE_SUSPEND;
    }

    while(tempPointOfInsert!=NULL)
    {

    if(current_suspend_list_pcb->time_atMost_resume<=tempPointOfInsert->time_atMost_resume){
    //tempLastWaitList->next=tempPointOfInsert;
    if(tempBeforeInsert!=NULL){
        tempBeforeInsert->next=current_suspend_list_pcb;
        current_suspend_list_pcb->next=tempPointOfInsert;
    }
    else{
        *s=current_suspend_list_pcb;
        (*s)->next=tempPointOfInsert;
        //tempSecondLastWaitList->next=NULL;
    }
    goto DONE_SUSPEND;
}

    tempBeforeInsert=tempPointOfInsert;
    tempPointOfInsert=tempPointOfInsert->next;
}
    if(tempPointOfInsert!=NULL)
        tempPointOfInsert->next=current_suspend_list_pcb;
    else
        tempBeforeInsert->next=current_suspend_list_pcb;


    DONE_SUSPEND:return;

}


void delay(unsigned int t){
    clock_t ticks1, ticks2;

	ticks1=clock();
	ticks2=ticks1;
	while(((ticks2-ticks1)*1000/CLOCKS_PER_SEC)<t)
		ticks2=clock();
}



inline void RTOS_debugger(){


}

#endif
