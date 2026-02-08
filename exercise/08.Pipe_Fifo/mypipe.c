#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(){

    printf("Demo Pipe\n");

    pid_t pid_1, pid_2;
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("Creat Pipe error \n") ;
        exit(-1);
    }
    pid_1 = fork();
    pid_2 = fork();

    if(pid_1 == 0)
    {  
        close(pipefd[0]);  // close read
        dup2(pipefd[1],STDOUT_FILENO);
        execlp("ls","ls","-l",NULL);
        exit(1);
    }
    else if(pid_2 == 0)
    {
        close(pipefd[1]); // close write
        dup2(pipefd[0],STDIN_FILENO);        
        execlp("wc","wc","-l",NULL);
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    

    wait(NULL);
    wait(NULL);


    return 0;
}