#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>
#include "global.h"


void process_init();
void process1();
void process2();

void process_1s();
void process_2s();
void process_3s();


int main()
{

    time_init=clock();

    process_control_block=(struct pcb *)calloc(MAX_PROCESS, sizeof(struct pcb));

    mTaskCreate(process_init, HIGH);
    mTaskCreate(process1, HIGH);
    mTaskCreate(process2, LOW);

    //mTaskCreate(process_1s, HIGH);
    //mTaskCreate(process_2s,  HIGH);
    //mTaskCreate(process_3s, HIGH);

    scheduler(&topWaitList, &topSuspendList);

    return 0;
}

void process_init(){
    printf("Initialising\n");
    wait(&topWaitList, 5000);
}

void process1(){
    printf("hello world\n");

    wait(&topWaitList, 2000);

}

void process2(){
    printf("hello linux\n");

   wait(&topWaitList, 3000);

}

void process_1s(){
    printf("bye RTOS\n");

    suspend(&topSuspendList,1 );
}

void process_2s(){
    printf("bye linux\n");

    suspend(&topSuspendList, 2);
}

void process_3s(){
    printf("bye world\n");

    suspend(&topSuspendList, 3);
}


