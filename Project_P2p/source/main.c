#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include "../include/getcomand.h"
#include "../include/basic_infomation.h"
/*Variable global*/
command_t  choice_user = CMD_HELP; 
uint16_t Port; 
information_self_socket self;
information_connect_socket *connect_socket;
int number_of_connection  = 0 ;

/*Fuction*/
void *task_client(void *index);
void *task_server(void *index);
int check_condition_port_is_correct(char *Port_cmd);
/*main branch */
int main(int argc, char *argv[])
{
    char PORT_cmd[100];
    pthread_t serve, client;

    /*Check condition before */
    /*Check argument*/
    if(get_parameter_initial(argc,argv,PORT_cmd,BUFFER_SIZE) != SUCCESS)
    {
        fprintf(stderr,"Format wrong!!\n");
        return FAIL;

    }
    /* Check port */
    if(check_condition_port_is_correct(PORT_cmd) != 0)
    {
        fprintf(stderr,"PORT wrong!! \n");
        return FAIL;
    }
    /*Malloc socket array*/
    if(malloc_connect_socket() != 0)
    {
        fprintf(stderr,"socket error\n");
        return FAIL;
    }
    /*initial infomation*/
    self.address.sin_addr.s_addr = inet_addr(get_local_ip());
    self.address.sin_port = htons(Port);
    self.status_serve = -1 ; 

    fflush(stdin);
    /*Display all choice*/
    fuction_display_help();

    /*Creat 2 thread for client and server*/
    pthread_create(&serve,NULL,task_server, &Port);
    pthread_create(&client,NULL,task_client,&Port);
    /*Main */
    while( choice_user != CMD_EXIT)
    {
        sleep(1);
        printf("\nEnter the command: ");
        fflush(stdin);
        char *command;
        command = getcommand();
        Check_Command(Port,command,&choice_user);
       
    }
    /*Wait thread end*/
    pthread_join(serve,NULL);
    pthread_join(client,NULL);

    return 0;
}
/*Client thread*/
void *task_client(void *index)
{   
   /*wait any client data*/
    while( choice_user != CMD_EXIT)
    {
    
        tcp_stream_client();
    }
    return NULL;
}
/*server thread*/
void *task_server(void *index)
{
    /*Check Port free*/    
    if(is_port_free(self.address.sin_port) != PORT_FREE )
    {
        fprintf(stderr,"Port problem");
        close(self.status_serve);
        choice_user = CMD_EXIT;
        return NULL ;
    }
    /*Creat server*/
    if(server_creat(htons(self.address.sin_port)) == 1)
    {
        fprintf(stderr,"connet fail!!\n");
        close(self.status_serve);
        choice_user = CMD_EXIT;
        return  NULL;
    }
    /*wait any serve*/
    while( choice_user != CMD_EXIT)
    {
        tcp_stream_server();
    }
    return NULL;
}

int check_condition_port_is_correct(char *PORT_cmd)
{
    /*check Port free */
    if(is_number(PORT_cmd) != SUCCESS)
    {
        fprintf(stderr,"Format wrong!! \n");
        return FAIL;
    }
    Port = atoi(PORT_cmd);
    /*check port permission*/
    if(Check_Port_Permission(Port) != 0)
    {
        fprintf(stderr,"Port can't access\n");
        return FAIL;
    }
    
    return SUCCESS;
}