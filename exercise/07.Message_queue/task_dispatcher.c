#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* QUEUE_NAME = "/my_task_queue";
mqd_t myqueue;

int is_number(const char *str)
{
    if (*str == '\0') return -1; 

    while (*str)
    {
        if (!isdigit(*str))
            return -1;
        str++;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    if(argc <3 && is_number(argv[1]) != 0)
    {
        fprintf(stderr,"\n Wrong format\n");
        exit(1);
    }
     /*open queue*/
    //    If O_CREAT is specified in oflag, then two additional arguments
    //    must be supplied.  The mode argument specifies the permissions to
    //    be placed on the new queue,
    myqueue = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY ,0666, NULL);
    
    if(myqueue == (mqd_t)-1)
    {
        perror("-1 queue\n");
        exit(1);
    }

    int priority = atoi(argv[1]);

    mq_send(myqueue,argv[2],strlen(argv[2]) + 1 ,priority);
    printf("\n send buffer <%s> success\n ",argv[2]);

    mq_close(myqueue);
    return 0;
}