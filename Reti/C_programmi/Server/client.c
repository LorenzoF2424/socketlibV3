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


int main(int argc, char **argv) {

    int s=0;
    if ((s=socket(AF_INET, SOCK_STREAM, 0))<0) {
        perror("Errore: Creazione Socket non riuscita");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sin={
        .sin_addr.s_addr = inet_addr("127.0.0.1"), 
        .sin_family = AF_INET,
        .sin_port = htons(7777)
    };
    
    if (connect(s, (struct sockaddr*)&sin, sizeof(sin))<0) {
        close(s);
        perror("Errore: Apertura Connessione non riuscita");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ+1];
    int byteLetti=0;
    printf("Messaggio ricevuto dal Server:\n");
    while((byteLetti=read(s, buffer, sizeof(buffer)))>0)
        write(1, buffer, byteLetti);
    close(s);


    printf("\n");
    return 0;
}