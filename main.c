#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include<math.h>
#include "global.h"

void process_init();
void process1();
void process2();

void process_1s();
void process_2s();
void process_3s();


int main()
{
//*current_pcb=(struct pcb *)malloc(sizeof(struct pcb));
    time_init=clock();

    process_control_block=(struct pcb *)calloc(MAX_PROCESS, sizeof(struct pcb));


    initial_RTOS(process_init, "periodic", HIGH);
    initial_RTOS(process1, "periodic", HIGH);
    initial_RTOS(process2, "periodic", LOW);
    initial_RTOS(process_init, "non-periodic", HIGH);

    scheduler(&topWaitList, &topSuspendList);

    return 0;
}

void process_init(){
    printf("Initialising\n");
    wait(&topWaitList, 5);
}

void process1(){
    printf("hello world\n");

    wait(&topWaitList, 2);

}

void process2(){
    printf("hello linux\n");

    wait(&topWaitList, 3);

}

void process_1s(){
    printf("bye RTOS\n");

    wait(&topWaitList, 3);
    //suspend();
}

void process_2s(){
    printf("bye linux\n");

    wait(&topWaitList, 3);
    //suspend();
}

void process_3s(){
    printf("bye world\n");

    wait(&topWaitList, 3);
    //suspend();
}



