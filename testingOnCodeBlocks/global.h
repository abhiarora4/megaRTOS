#ifndef GLOBAL_H
#define GLOBAL_H


#define CLOCKS_PER_MSEC CLOCKS_PER_SEC*1000
#define MAX_PROCESS 10

struct pcb *process_control_block;
struct pcb *current_pcb;
struct wait_list *topWaitList=NULL;
struct suspend_list *topSuspendList=NULL;
clock_t time_init;


#endif
