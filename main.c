#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include<math.h>
#include "global.h"

void process_init();
void process1();
void process2();


int main()
{
//*current_pcb=(struct pcb *)malloc(sizeof(struct pcb));
    time_init=clock();

    process_control_block=(struct pcb *)calloc(MAX_PROCESS, sizeof(struct pcb));


    initial_RTOS(process_init, "periodic", HIGH);
    initial_RTOS(process1, "periodic", HIGH);
    initial_RTOS(process2, "periodic", LOW);

    scheduler(&topWaitList);

    return 0;
}

void process_init(){
    printf("Initialising\n");
    wait(&topWaitList, 5);
}

void process1(){
    printf("hello world\n");
    func=process2;
    wait(&topWaitList, 2);
    //scheduler();
}

void process2(){
    printf("hello linux\n");
    func=process1;
    wait(&topWaitList, 3);
    //scheduler();
}

