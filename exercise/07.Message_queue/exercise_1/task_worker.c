#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const char* QUEUE_NAME = "/my_task_queue";
mqd_t myqueue;

void Handle_signal(int sig)
{
    printf("\n stop receive from queue\n");

    mq_unlink(QUEUE_NAME);
    mq_close(myqueue);
  
}

int main()
{   
    struct mq_attr attr;
   // char buffer[1024];
    unsigned int priority;

    printf("\n Task worker\n");

    /*signal*/
    signal(SIGINT,Handle_signal);
    // mq_unlink(QUEUE_NAME);

    /*open queue*/
    myqueue = mq_open(QUEUE_NAME,  O_RDONLY );

    if(myqueue == (mqd_t) -1)
    {
        perror("Fail queue\n");
        exit(1);
    }

    mq_getattr(myqueue,&attr);

    printf("current message size : %ld\n",attr.mq_curmsgs);
    char buffer[attr.mq_msgsize + 1];

    while( mq_receive(myqueue,buffer,sizeof(buffer),&priority) != -1)
    {
        
        printf("Processing task (Priority: <%d>): <%s>.\n",priority,buffer);
        sleep(1);
        
    }

    printf(" \n end of program\n");
 
    return 0;
}