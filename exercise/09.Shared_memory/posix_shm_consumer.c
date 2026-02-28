#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>  
#include <fcntl.h>
#include "product.h"

int main(){

   printf("Consumer task\n");

    Product product = {
        .ID = 15,
        .name = "Chocolate",
        .price = 32,
    };
   
    int shm_fd = shm_open(filename,O_RDWR | O_CREAT, 0666 );
    
    if (shm_fd == -1) {
        perror("open");
        return 1;
    }
    ftruncate(shm_fd,SIZE);
    
    Product *ptr = mmap(NULL , SIZE, PROT_WRITE  , MAP_SHARED ,shm_fd , 0); 
      if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    *ptr = product;
    close(shm_fd);

    printf("Write success \n");
    munmap(ptr,SIZE);

    return 0;
}