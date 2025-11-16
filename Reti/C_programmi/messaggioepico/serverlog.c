#include "lib.h"




int main(int argc, char **argv) {

    
   int sock_fd;
   int sock;
   struct sockaddr_in addr;
   socklen_t addr_len = sizeof(addr);
   if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
       perror("socket failed");
       return -1;
   }

   memset(&addr, 0, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_port = htons(PORT);
   addr.sin_addr.s_addr = htonl(INADDR_ANY);

   // Bind and listen
   if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
       perror("bind failed");
       return -2;
   }
  
   FILE *f = fopen("log.txt","a");


    logl linea;

    while (true) {
        recvfrom(sock_fd, (void*)&linea, sizeof(logl), 0, (struct sockaddr*)&addr, &addr_len);
        fprintf(f, linea.logline);


    }
    close(f);
    return 0;
}










