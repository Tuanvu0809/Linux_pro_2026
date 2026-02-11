#ifndef GET_COMMAND_H
#define GET_COMMAND_H

#include <string.h>
#include <ctype.h>
#include "basic_infomation.h"
#include "communicate.h"
#include <stdlib.h>

#define MAX_PARAMETER   10
#define MAX_SIZE     50
#define PORT_MIN    1024 // PORT smallest  can accessible
#define PORT_MAX    49152 // PORT bigger can accessible

/*Status check any fail and success*/
typedef enum{
    FAIL = -1,
    SUCCESS = 0,
} status;
/*choice user can usr*/
typedef enum{
    CMD_HELP  = 0,
    CMD_DISPLAY_IP,
    CMD_DISPLAY_PORT,
    CMD_CONNECT,
    CMD_LIST,
    CMD_SEND_MESSAGE,
    CMD_TERMINAL,
    CMD_UNKNOWN,
    CMD_EXIT,
} command_t; 

int get_parameter_initial(int argc, char *argv[], char *buffer, int buffer_size) ;
int is_number(const char *str);
int Check_Port_Permission(uint16_t Port);
char *getcommand();

void Check_Command(uint16_t Port , char *buffer, command_t *choice );


#endif