#define SIZE 64

struct mQueueHandler{
    char front;
    char rear;
    int queue[SIZE];
    struct mQueueBlockList *top;
};

struct mQueueBlockList{
    struct pcb *pcb_block_queue;
    struct mQueueBlockList *next;

};

struct mQueueHandler * mQueueCreate(){
        struct mQueueHandler *ret = (struct mQueueHandler *)malloc(sizeof(struct mQueueHandler));

        return ret;
}

char mQueueEnque(struct mQueueHandler *k, int val){
    char f=k->front;
    char r=k->rear;

    if(((r+1) % SIZE)==f)
        return 1;
    else{
        r=(r+1)%SIZE;
        k->queue[r]=val;
        k->rear=r;

        return 0;
    }
}

char mQueueDeque(struct mQueueHandler *k, int *val){
    char f=k->front;
    char r=k->rear;

    if(f==r)
        return 1;
    else{
        f=(f+1)%SIZE;
        *val=k->queue[f];
        k->front=f;

        return 0;
    }


}

char mQueuePeek(struct mQueueHandler *k, int *val){
    char f=k->front;
    char r=k->rear;

    if(f==r)
        return 1;
    else{
        f=(f+1)%SIZE;
        *val=k->queue[f];
        return 0;
    }
}

char mQueueDelete(struct mQueueHandler *k){
    free(k);
}

char mQueuePut(struct mQueueHandler *k, int *val){
    char f=k->front;
    char r=k->rear;

    if(((r+1) % SIZE)==f)
        return 1;
    else{
        r=(r+1)%SIZE;
        k->queue[r]=val;
        k->rear=r;

        return 0;
    }
}

char mQueueGet(struct mQueueHandler *k, int *val){
    char f=k->front;
    char r=k->rear;

    if(f==r)
        return 1;
    else{
        f=(f+1)%SIZE;
        *val=k->queue[f];
        k->front=f;

        return 0;
    }

}

char mQueueIsEmpty(struct mQueueHandler *k){

    if(k->front==k->rear)
        return 0;
    else{
        return 1;
    }
}
