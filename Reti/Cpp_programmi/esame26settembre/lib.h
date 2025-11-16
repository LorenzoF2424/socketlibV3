#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdbool.h>
#include <thread>
#include <math.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;



typedef struct {
    int id;
    char nome[32];
    char message[256];
    char ipMittente[32];
    char ipDestinatario[32];
    int portaMittente;
    int portaDestinatario;
    bool isAck;
    
}packet;

class Socket {
    public:
        int sockfd;
        string ip;
        int portIn;
        int portOut;
        string protocoll;
        sockaddr_in connIn;
        sockaddr_in connOut;
        sockaddr_in mittente;


    Socket(string PROTOCOLL) : protocoll(PROTOCOLL) {

        int type = protocoll == "TCP" ? SOCK_STREAM : SOCK_DGRAM;
        this->sockfd=socket(AF_INET,type,0);
    }
    void bindRecv(string ip,int port) {
        this->ip = ip;
        this->portIn = port;
        memset(&connIn, 0, sizeof(connIn));
        connIn.sin_family = AF_INET;
        inet_pton(AF_INET, this->ip.c_str(), &connIn.sin_addr);
        connIn.sin_port = htons(portIn);

         if(bind(sockfd, (struct sockaddr*)&connIn, sizeof(connIn)) < 0) 
            perror("bind failed");


    }

    packet recvP() {

        packet p;
        socklen_t clientInLen = sizeof(mittente);
        recvfrom(sockfd, &p, sizeof(p), 0,
                              (struct sockaddr*)&mittente, &clientInLen);


        return p;
    }

    void setOut(string ip,int port) {

        memset(&connOut, 0, sizeof(connOut));
        connOut.sin_family = AF_INET;
        inet_pton(AF_INET, ip.c_str(), &connOut.sin_addr);
        connOut.sin_port = htons(port);


    }

    void sendP(packet p) {

         sendto(sockfd, &p, sizeof(p), 0,
          (struct sockaddr*)&connOut, sizeof(connOut));

    }

    void close() {::close(this->sockfd);}
    
};


packet pBase;
packet bufferP;
void handleRecv(Socket *s) {

    while (true) {

        bufferP=s->recvP();
        packet p=pBase;
        if (bufferP.isAck && bufferP.portaDestinatario == pBase.portaMittente) {
            cout<<"Ack per il messaggio "<<bufferP.id<<" ricevuto!!"<<endl;
        } else if (bufferP.portaDestinatario!=pBase.portaMittente) {
            cout<<"Ricevuto pacchetto per "<<bufferP.nome<<"("<<bufferP.portaDestinatario<<"), inoltrando....."<<endl;
            
            s->setOut("127.0.0.1",bufferP.portaDestinatario);
            s->sendP(bufferP);

        } else {
            cout<<endl<<">["<<bufferP.nome<<"] "<<bufferP.message<<endl;
            p.portaDestinatario=bufferP.portaMittente;
            p.id=bufferP.id;
            p.isAck=true;
            s->setOut("127.0.0.1",p.portaDestinatario);
            s->sendP(p);
        }

    }


    
}