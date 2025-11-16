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
    
 
    while (true) {

        client_info *client = malloc(sizeof(client_info));
       socklen_t addrlen = sizeof(client->address);


        if ((client->sockfd=accept(sFD, (struct sockaddr*)&client->address, &addrlen))<0) {
            free(client);
            perror("Errore: Client non accetato");
            continue;
        }


        pthread_t thread_id;
        if(pthread_create(&thread_id, NULL, handle_client, (void *)client) != 0) {
            perror("pthread_create");
            close(client->sockfd);
            free(client);
        } else {
            pthread_detach(thread_id);
        }



        

    }


    close(sFD);
    pthread_mutex_destroy(&clients.mutex);
   

    return 0;
}