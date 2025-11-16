#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <cstring>
#include <vector>
#include <fstream>
#include "socketlib.h"
using namespace std;

#define NOME_LEN 32
#define MSG_LEN 256

#pragma pack(push, 1)
typedef struct {
   char nome[NOME_LEN];
   int valore;
   bool isAperta;
}packetS;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
   char msg[MSG_LEN];
}packetLog;
#pragma pack(pop)



/*
typedef struct {
   int sockfd;
   sockaddr_in address;
}client_info;

void setConn(sockaddr_in *addr, string ip, int port) {

   memset(addr, 0, sizeof(*addr));
   addr->sin_family = AF_INET;
   addr->sin_port = htons(port);


   if (ip=="ANY") addr->sin_addr.s_addr = htonl(INADDR_ANY);
   else inet_pton(AF_INET, ip.c_str(), &addr->sin_addr.s_addr);

}

class Socket {
   public:
      int sockfdTCP;
      int sockfdUDP;
      struct sockaddr_in connTCP;
      sockaddr_in connUDPIn;
      sockaddr_in connUDPOut;
      vector<client_info> client_list;
      pthread_mutex_t mutex;
   


      Socket() { int santoro=0; pthread_mutex_init(&mutex,NULL);}
      void bindTCP(string ip, int port) {

         if ((this->sockfdTCP = socket(AF_INET, SOCK_STREAM, 0))<0) {
            perror("Errore: Creazione Socket TCP Fallita.");
            exit(EXIT_FAILURE);

         }

         int opt=1;
         setsockopt(this->sockfdTCP, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
         
         
         setConn(&this->connTCP, ip, port);
         socklen_t len=sizeof(this->connTCP);
         if ((bind(this->sockfdTCP, (struct sockaddr*)&this->connTCP, len)) < 0) {
            close(this->sockfdTCP);
            perror("Errore: Bind TCP Fallita.");
            exit(EXIT_FAILURE);
         }



      }

      void connectTCP(string ip, int port) {

         if ((this->sockfdTCP = socket(AF_INET, SOCK_STREAM, 0))<0) {
            perror("Errore: Creazione Socket TCP Fallita.");
            exit(EXIT_FAILURE);

         }

         setConn(&this->connTCP, ip, port);
         if (connect(this->sockfdTCP, (struct sockaddr*)&this->connTCP, sizeof(this->connTCP)) < 0) {
            close(this->sockfdTCP);
            perror("Errore: Connect Fallita.");
            exit(EXIT_FAILURE);

         }

      }

      void listenTCP(int MAX_CLIENT) {

         if (listen(this->sockfdTCP, MAX_CLIENT) < 0) {
            close(this->sockfdTCP);
            perror("Errore: Listen Fallita.");
            exit(EXIT_FAILURE);

         }

      }
   
      int acceptTCP() {

         struct sockaddr_in sock;
         socklen_t lenS = sizeof(sock);
         int newsockfd=0;
         if ((newsockfd=accept(this->sockfdTCP,(struct sockaddr*)&sock,&lenS))<0) {
            perror("Errore: Accept Fallita.");
         }

         client_info c;
         c.sockfd=newsockfd;
         c.address=sock;
         this->client_list.push_back(c);
         return this->client_list.size()-1;


      }

      template<typename T>
      bool recvTCP(int i, T *p) {

         int numBytes=0;

         if((numBytes = recv(this->client_list[i].sockfd,(void*)p,sizeof(T),0))<=0) {

            close(this->client_list[i].sockfd);
            if (numBytes==0) {

               cout<<"client "<<i<<" disconnesso"<<endl;
               

            } else
            perror("Errore: Recv Fallita.");
            return false;

         }

         return true;

      }

      template<typename T>
      bool sendTCP(int i, T *p) {

         if((send(this->client_list[i].sockfd, p, sizeof(*p), 0))<0) 
            return false;
         
         return true;

      }

      template<typename T>
      bool sendTCP(T *p) {

         if((send(this->sockfdTCP, p, sizeof(*p), 0))<0) 
            return false;
         
         return true;
      }


      void bindUDP(string ip, int port) {

         if ((this->sockfdUDP = socket(AF_INET, SOCK_DGRAM, 0))<0) {
            perror("Errore: Creazione Socket UDP Fallita.");
            exit(EXIT_FAILURE);

         }

         int opt=1;
         setsockopt(this->sockfdUDP, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
         
         
         setConn(&this->connUDPIn, ip, port);
         if ((bind(this->sockfdUDP, (struct sockaddr*)&this->connUDPIn, sizeof(this->connUDPIn))) < 0) {
            close(this->sockfdUDP);
            perror("Errore: Bind UDP Fallita.");
            exit(EXIT_FAILURE);
         }

      }

      template<typename T>
      T recvUDP() {
         T p;
         sockaddr_in Mittente;
         socklen_t socklen = sizeof(Mittente);
         if((recvfrom(this->sockfdUDP, (void*)&p, sizeof(T), 0,
                                       (struct sockaddr*)&Mittente,&socklen))<0) {
            perror("Errore: Recv UDP Fallita.");

         }

         return p;

      }


      void setUDPOut(string ip, int port) {setConn(&this->connUDPOut,ip,port);}

      template<typename T>
      void sendUDP(T *p) {
         
         socklen_t socklen = sizeof(this->connUDPOut);
         if((sendto(this->sockfdUDP,(void*)p, sizeof(T), 0,(struct sockaddr*)&this->connUDPOut,socklen))<0) {
            perror("Errore: Recv UDP Fallita.");

         }

         

      }


      void closeTCP() {close(this->sockfdTCP);}
      void closeUDP() {close(this->sockfdUDP);}
      void closeAll() {close(this->sockfdTCP); close(this->sockfdUDP);}
};


*/