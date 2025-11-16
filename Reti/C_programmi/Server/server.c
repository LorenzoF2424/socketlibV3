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

    int sS=0, porta=7777;
    struct sockaddr_in sin;
    
    if ((sS=socket(AF_INET, SOCK_STREAM, 0))<0) {
        perror("Errore: socket era gay");
        exit(EXIT_FAILURE);
    }

    memset(&sin, 0, sizeof(sin));
    sin = (struct sockaddr_in){
        .sin_addr.s_addr = inet_addr("0.0.0.0"),
        .sin_family = AF_INET,
        .sin_port = htons(porta),
    };

    int opt = 1;
    setsockopt(sS, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(sS,(struct sockaddr*) &sin, sizeof(sin))<0) {
        close(sS);
        perror("Errore: bind trans");
        exit(EXIT_FAILURE);
    }

    if (listen(sS, 1)<0) {
        close(sS);
        perror("Errore: listen non binary");
        exit(EXIT_FAILURE);
    }

    int cS=0;
    struct sockaddr_in csin;
    socklen_t csin_len = sizeof(csin);
    printf("Server aperto su porta: %d", porta);

    while (true) {

        if ((cS=accept(sS, (struct sockaddr*)&csin, &csin_len))<0) {
            perror("Errore: Client se la sta minando");
            continue;
        }
        printf("Client conesso a ip: %s", inet_ntoa(csin.sin_addr));
        pid_t pid= fork();
        if (pid<0) {
            perror("Errore: Figlio down");
            close(cS);
        } else if (pid==0) { // pid=0 -> figlio se no è padre

            close(sS);
            char ciao[] = "Hello from server\n";
            send(cS, ciao, strlen(ciao), 0);
                    printf("messaggio inviato a: %s", inet_ntoa(csin.sin_addr));

            shutdown(cS, SHUT_WR);
            close(cS);
            exit(EXIT_SUCCESS);

        } else close(cS);

        

    }

    close(sS);
    printf("\n");
    return 0;
}