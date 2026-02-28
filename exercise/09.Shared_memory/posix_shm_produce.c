#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>  
#include <fcntl.h>
#include "product.h"
#include <stdlib.h>
int main(){

   printf("Producer task\n");

   int shm_fd = shm_open(filename, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    Product *ptr = mmap(NULL , SIZE, PROT_READ , MAP_SHARED ,shm_fd , 0); 
    
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Reader: %d %s %d\n",ptr->ID,ptr->name,ptr->price);

    close(shm_fd);
    munmap(ptr,SIZE);
    shm_unlink(filename);
    
    return 0;
}