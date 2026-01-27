#include <stdio.h>
#include <fcntl.h>
#include "../include/communicate.h"

extern information_self_socket self;
extern information_connect_socket *connect_socket;
extern command_t  choice_user ; 
extern int number_of_connection ;

int is_port_free(uint16_t Port)
{
    int sockfd;
    struct sockaddr_in addr;
    int opt = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return PORT_ERROR;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(Port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sockfd);

        if (errno == EADDRINUSE)
            return PORT_BUSY;   
        else
            return PORT_ERROR;  
    }

    close(sockfd);
    return PORT_FREE;
}

static void handle_pool_event(int index)
{

}

static void pool_serve(int server_fd)
{
   
    char buffer[BUFFER_SIZE];
    struct pollfd fds[MAX_CLIENT + 1];
    int nfds = 0;

    fds[nfds].fd = server_fd;
    fds[nfds].events = POLLIN;
    nfds++;

    if (fds[0].revents & POLLIN) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);

        int client_fd = accept(self.status_serve, (struct sockaddr *)&addr, &len);
        
        if (client_fd >= 0 && number_of_connection < MAX_CLIENT)
        {
            connect_socket[number_of_connection].status = client_fd;
            connect_socket[number_of_connection].address = addr;
            fprintf(stdout,"\n Accept a connection from address %s set up at %d\n",inet_ntoa(connect_socket[number_of_connection].address.sin_addr),htons(connect_socket[number_of_connection].address.sin_port));
            (number_of_connection)++;

        } 

    }

    for (int i = 0; i < number_of_connection; i++)
    {
        fds[nfds].fd = connect_socket[i].status;
        fds[nfds].events = POLLIN;
        nfds++;
    }

    if (nfds == 0)
    {
        return;
    }

    int ready = poll(fds, nfds, -1);
    if (ready < 0) {
        perror("poll");
        return;
    }

    for (int i = 1; i < nfds; i++) {

        if (fds[i].revents & POLLIN) {

            int idx = i - 1; 
            int fd = connect_socket[idx].status;
            int n = read(fd, buffer, BUFFER_SIZE - 1);

            if (n > 0) {
                buffer[n] = '\0';

                if(strcmp(buffer,"terminal") == 0 && n > 0)
                {
                    fprintf(stdout,"\nTerminal because client  %d terminal\n",idx);
                    Tcp_terminal(idx);
                }
                else if(strcmp(buffer,"exit") == 0 && n > 0)
                {
                    fprintf(stdout,"\nTerminal because client %d exit\n",idx);
                    Tcp_terminal(idx);
                }
                else
                {

                    printf("\n******************************\n");
                    printf("Message receive from %s\n" , inet_ntoa(connect_socket[idx].address.sin_addr) );
                    printf("From Port %d\n",htons(connect_socket[idx].address.sin_port));
                    printf("Message: %s\n", buffer);
                    printf("******************************\n");
                }

            }
            else if (n <= 0)
             {
                close(fd);
                Tcp_terminal(idx);
                continue;
            }

        }
    }
  
}

static void pool_client()
{
    struct pollfd fds[MAX_CLIENT];
    char buffer[BUFFER_SIZE];

    int nfds = 0;

    for (int i = 0; i < number_of_connection; i++) {
        if (connect_socket[i].status >= 3) {
            fds[nfds].fd = connect_socket[i].status;
            fds[nfds].events = POLLIN;
            nfds++;
        }
    }

    if (nfds == 0) 
    {
        return;
    }

    int ready = poll(fds, nfds, -1); 
    if (ready < 0) {
        perror("poll");
        return;
    }
    
    for (int i = 0; i < nfds; i++) {

        if (fds[i].revents & POLLIN) {

            int fd = fds[i].fd;
            int n = read(fd, buffer, BUFFER_SIZE - 1);

            if (n > 0) 
            {
                buffer[n] = '\0';

                if(strcmp(buffer,"terminal") == 0)
                {
                    fprintf(stdout,"\nTerminal because serve %d terminal\n",i);
                    Tcp_terminal(i);
                }
                else if(strcmp(buffer,"exit") == 0 )
                {
                    fprintf(stdout,"\nTerminal because serve %d exit\n",i);
                    Tcp_terminal(i);
                }
                else if(strcmp(buffer,"connect") == 0 )
                {
                    printf("Accept the connect\n");   
                }
                else
                {
                    printf("\n******************************\n");
                    printf("Message receive from %s\n" , inet_ntoa(connect_socket[i].address.sin_addr) );
                    printf("From Port %d\n",htons(connect_socket[i].address.sin_port));
                    printf("Message: %s\n", buffer);
                    printf("******************************\n");
                }

            }
            else if (n <= 0)
            {
                close(fd);
                Tcp_terminal(i);
                continue;
            }        
          
        }
    }
}

int server_creat(uint16_t PORT_CONNECT)
{

    //Creat socket
    self.status_serve = socket(AF_INET,SOCK_STREAM,0);
    if( self.status_serve < 0)
    {
        perror("fail creat serve socket \n");
        choice_user = CMD_EXIT;
        return 1;
    }
    
    memset(&self.address, 0, sizeof(self.address));
    self.address.sin_family = AF_INET;
    self.address.sin_port = htons(PORT_CONNECT);
    self.address.sin_addr.s_addr = INADDR_ANY;
 
    if((bind(self.status_serve,(struct sockaddr*)&self.address, sizeof(self.address)) < 0)   )
    {
        perror("fail bind serve address\n");
        close(self.status_serve);
        choice_user = CMD_EXIT;
        return 1;
    }

    //listen 
    if( listen(self.status_serve,DEVICE) < 0)
    {
        perror("fail listen\n");
        close(self.status_serve);
        return 1;
    }
    
    return SUCCESS;
}

int Client_creat(uint16_t PORT_CONNECT , char *ip)
{

    information_connect_socket *connect_other;
    /* malloc sockaddr_in */
    connect_other = (information_connect_socket *)malloc(sizeof(information_connect_socket));
    if (!connect_other) {
        perror("malloc failed");
         return FAIL;;
    }
      socklen_t len = sizeof(connect_other->address);

    // 1. socket() - Create socket
    self.status_client = socket(AF_INET, SOCK_STREAM, 0);
    if (self.status_client < 0) {
        perror("socket failed");
        free(connect_other);
         return FAIL;;
    }

    // 2. Setup server address
    memset(connect_other, 0, sizeof(struct sockaddr_in));
    connect_other->address.sin_family = AF_INET;
    connect_other->address.sin_port = htons(PORT_CONNECT);
    connect_other->status = -1;

    // Convert IP address
    if (inet_pton(AF_INET, ip, &connect_other->address.sin_addr) <= 0) {
        perror("Invalid address");
        close(self.status_client);
        free(connect_other);
        return FAIL;;
    }

    if (connect(self.status_client,  (struct sockaddr *) &connect_other->address,  sizeof(struct sockaddr_in)) < 0) {
        perror("connect failed");
        close(self.status_client);
        free(connect_other);
         return FAIL;;
    }

    connect_other->status = self.status_client;
    connect_socket[number_of_connection] = *connect_other;  
    
    number_of_connection ++;
    free(connect_other);

    sleep(1);

    return SUCCESS;
}

int Send_message_to_connect(int index, const char *message, const char *fuction)
{
    ssize_t sent;
    int fd = connect_socket[index].status;

    if (fd < 3) {
        fprintf(stderr, "invalid socket fd=%d\n", fd);
        return FAIL;
    }
    if ( strcmp(fuction,"send")==0)
    {
        sent = send(fd, message, strlen(message), 0);
        if (sent < 0) {
            perror("send failed");
            return FAIL;
        }
    }
    else if (strcmp(fuction,"terminal") == 0)
    {
        sent = send(fd,fuction,strlen(fuction),0);
        
        if (sent < 0) {
            perror("send failed");
            return FAIL;
        }
    } 
    else if(strcmp(fuction,"exit") == 0)
    {
        sent = send(fd,fuction,strlen(fuction),0);
        
        if (sent < 0) {
            perror("send failed");
            return FAIL;
        }
    }
    else if(strcmp(fuction,"connect") == 0)
    {
        sent = send(fd,fuction,strlen(fuction),0);
        
        if (sent < 0) {
            perror("send failed");
            return FAIL;
        }
    }

    return SUCCESS;

}

void Tcp_terminal(int index)
{
    if (index < 0 || index >= number_of_connection)  return;

    int fd = connect_socket[index].status;

    if (fd >= 0) {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }

    for (int i = index; i < number_of_connection - 1; i++) {
        connect_socket[i] = connect_socket[i + 1];
    }

    number_of_connection--;
} 

void Tcp_stream_disconnect()
{

    if (self.status_client >= 0)
        shutdown(self.status_client, SHUT_RDWR);

    if (self.status_serve >= 0)
        shutdown(self.status_serve, SHUT_RDWR);
   
    if (connect_socket != NULL){    
        shutdown(connect_socket->status,SHUT_RDWR);
        free(connect_socket);
    }    
    
}

void tcp_stream_server()
{
   pool_serve(self.status_serve);

}

void tcp_stream_client()
{
   pool_client();

}


void List_all_connect()
{
    
    printf("\n*************List******************************************************************\n");
    printf("index\2 | IP Address\2| Port\n ");
    for(int i =0 ; i< number_of_connection ; i++)
    {
       printf("%d  |  %s |  %d  \n",i,inet_ntoa(connect_socket[i].address.sin_addr),htons(connect_socket[i].address.sin_port) );
    
    }
    printf("\n************************************************************************************\n");
}


int malloc_connect_socket() {
    connect_socket = malloc(sizeof(information_self_socket) * MAX_CLIENT);
    if (connect_socket == NULL) return 1;
     return SUCCESS;
}
