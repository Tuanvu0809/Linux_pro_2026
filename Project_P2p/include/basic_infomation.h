#ifndef BASIC_INFORMATION_H
#define BASIC_INFORMATION_H
#include <string.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "communicate.h"
/*Marco*/
#define IP_BUFFER_SIZE 16   
/*fuction creat*/
int Check_socket_connect(char *ip , uint16_t Port);

void self_information_socket(uint16_t Port);
void fuction_display_help();
void fuction_display_ip();
void fuction_display_port();
void fuction_exit();
#endif