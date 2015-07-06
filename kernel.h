#ifndef _kernel_h_
#define _kernel_h_

#include<time.h>
#include "global.h"
#define CURRENT ((float)(clock()-time_init)/CLOCKS_PER_MSEC)


enum status {init, ready, running, waiting, suspended, finished};
enum priority {HIGH, LOW, MEDIUM};


struct pcb{
void (*pointer_to_function)();
enum status stat;
enum priority lev;

};

struct wait_list{
    struct pcb *pcb_wait;
    struct wait_list *next;

    int pid;

    float time_at_block;
    float del;
    float time_at_resume;
};

struct suspend_list{
    struct pcb *pcb_suspend;
    struct suspend_list *next;
    int pid;
    float time_atMost_resume;
    float time_at_suspend;
};


void (*func)();

void scheduler(struct wait_list **, struct suspend_list **);
void initial_RTOS(void (*process)(), char [], enum priority);

void wait(struct wait_list **, float );
void suspend(struct suspend_list **, float );

void delay(unsigned int);

void RTOS_debugger();




void scheduler(struct wait_list **t, struct suspend_list **s){
while(1){

//wait list
struct wait_list *temp=*t;
struct wait_list *tempx;

    while(temp->next!=NULL)
    {
      tempx=temp;
      temp=temp->next;
    }

struct wait_list *temp1=*t;
struct wait_list *temp2;
temp2=NULL;

while(temp1->next!=NULL)
{

if(temp->time_at_resume<temp1->time_at_resume){
    temp->next=temp1;
    if(temp2!=NULL)
        temp2->next=temp;
    else
        *t=temp;
    tempx->next=NULL;
    break;
}

temp2=temp1;
temp1=temp1->next;

}

//suspend list

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


//while(CURRENT<(*t)->time_at_resume);
/*
if(CURRENT>((*s)->time_atMost_resume)){
    func=(*s)->pcb_suspend->pointer_to_function;
    current_pcb=(*s)->pcb_suspend;
    func();
}

else*/ if(CURRENT>=(*t)->time_at_resume){


func=(*t)->pcb_wait->pointer_to_function;
current_pcb=(*t)->pcb_wait;
(*t)->pcb_wait->stat=running;

    //test addition
struct wait_list *tempy=*t;
    while(tempy->next!=NULL)
    {
      printf("%4.2f ->", tempy->time_at_resume);
      //printf("%d ->", temp->pid);
      tempy=tempy->next;
    }
printf("%4.2f \t", tempy->time_at_resume);
//printf("%d \t", temp->pid);
//ends

func();
}
/*
else{
    func=(*s)->pcb_suspend->pointer_to_function;
    current_pcb=(*s)->pcb_suspend;
    func();
}
*/
}
}



void initial_RTOS(void (*process)(), char arg[], enum priority lev){

    static int i;

    process_control_block[i].pointer_to_function=process;
    process_control_block[i].stat=ready;


    if(arg=="periodic")
    {
        struct wait_list *temp=(struct wait_list*)malloc(sizeof(struct wait_list));
        temp->pcb_wait=&(process_control_block[i]);
        temp->time_at_block=((float)(clock()-time_init))/CLOCKS_PER_SEC;
        temp->del=0;
        temp->pid=i;

        if(topWaitList==NULL){
            topWaitList=temp;
            temp->next=NULL;
        }
        else{
            temp->next=topWaitList;
            topWaitList=temp;
        }

    }
    else if(arg=="non-periodic"){

        struct suspend_list *temp=(struct suspend_list*)malloc(sizeof(struct suspend_list));
        temp->pcb_suspend=&(process_control_block[i]);
        temp->time_at_suspend=((float)(clock()-time_init))/CLOCKS_PER_SEC;
        temp->time_atMost_resume=temp->time_at_suspend;
        temp->pid=i;
        if(topSuspendList==NULL){
            topSuspendList=temp;
            temp->next=NULL;
        }
        else{
            temp->next=topSuspendList;
            topSuspendList=temp;
        }

    }

i++;
}


void wait(struct wait_list **t, float d){

    struct wait_list *temp=*t;
    while(temp->next!=NULL)
      temp=temp->next;

    temp->next=*t;
    *t=(*t)->next;
    temp=temp->next;
    temp->next=NULL;

    temp->time_at_block=((float)(clock() - time_init)/CLOCKS_PER_MSEC);
    temp->del=d;
    temp->time_at_resume=temp->time_at_block+d;


    temp->pcb_wait->stat=waiting;


}

void suspend(struct suspend_list **s, float del){
    struct suspend_list *temp=*s;
    while(temp->next!=NULL)
      temp=temp->next;

    temp->next=*s;
    *s=(*s)->next;
    temp=temp->next;
    temp->next=NULL;

    temp->time_at_suspend=((float)(clock() - time_init)/CLOCKS_PER_MSEC);
    temp->time_atMost_resume=temp->time_at_suspend+del;

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

