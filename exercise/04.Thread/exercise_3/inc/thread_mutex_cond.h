#ifndef MUTEX_COND_H
#define MUTEX_COND_H

typedef struct data_type{
    int data[10];
    int size;
    bool data_ready;
}data_type;

void Random_number(int *array,int quantity);
void *Consumer( void *arg);
void *Producer( void *arg);

#endif
