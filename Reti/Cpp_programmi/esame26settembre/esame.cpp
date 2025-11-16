#include <iostream>
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
#include "lib.h"
using namespace std;


int main(int argc, char **argv) {

    int port;
    cout<<"inserisci nome: ";
    cin>>pBase.nome;
    cout<<"inserisci porta di ricezione: ";
    cin>>pBase.portaMittente;
    Socket s("UDP");
    s.bindRecv("127.0.0.1",pBase.portaMittente);
    thread recvThread(handleRecv,&s);
    strcpy(pBase.ipMittente,"127.0.0.1");
    strcpy(pBase.ipDestinatario,"127.0.0.1");
    packet p;
    int id=1;
    int count=0;
    while (true) {

        p=pBase;
        p.id=id;
        cout<<"Inserisci messaggio da inviare: ";
        cin>>p.message;
        if (strcmp(p.message,"/exit")==0) break;

        cout<<"Inserisci la porta del destinatario finale del pacchetto: ";
        cin>>p.portaDestinatario;

        cout<<"Inserisci la porta del destinatario del pacchetto: ";
        cin>>port;
        
        



    
        s.setOut("127.0.0.1",port);
        count=0;
        while ((bufferP.id!=id || !bufferP.isAck) && count<5) {
            if (count>0) cout<<"Ack non ancora arrivato!!! Ritrasmettendo "<<count<<"....."<<endl;
            s.sendP(p);
            sleep(3);
            count++;
            //cout<<bufferP.id<<" "<<bufferP.isAck<<" "<<count<<endl;
            //cout<<(bufferP.id!=id)<<" "<<!bufferP.isAck<<" "<<count<<endl;
        }
        id++;
        bufferP.isAck=false;

    }


    return 0;
}


/*
Programmare una rete con interfaccia e controllore nel quale l'interfaccia spedisce pacchetti al controllore 
per essere smistati tramite id, ip,porta , e che dovranno ricevere un Ack di conferma, 
se cio non avverra si ripetera la trasmissione del messaggio ogni 3 secondi per 5 volte
TUTTO IN UDP
2h 30min di tempo limite
*/