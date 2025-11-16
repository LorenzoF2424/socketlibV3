#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "serverlib.h"


int main(int argc, char **argv) {

    int sFD = createServer(7777, 2);
    
    client_info *client = malloc(sizeof(client_info));

    handle_client(client,sFD);
           
     


        

    close(sFD);
    pthread_mutex_destroy(&clients.mutex);
   

    return 0;
}