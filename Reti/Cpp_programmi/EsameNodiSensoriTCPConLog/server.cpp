#include "lib.h"
Socket s;

void log(string s);
void handle_client(int index);

int main(int argc, char **argv) {

    s.bindTCP("ANY",7777);
    s.listenTCP(2);
    s.setUDPOut("127.0.0.1",8080);
    s.bindUDP("127.0.0.1",7000);
    log("[Server] Ascoltando per 2 su 7777");

    int index=0;
    bool finito=false;
    while(true) {
        sleep(1);
        index=s.acceptTCP();
        if(index<0) { index=0; continue;}
        log("[Server] Accettato Client");

        cout<<"accetato tizio "<<index<<endl;
        thread th(handle_client, index);
        th.detach();
        log("[Server] Creato Thread per Client");



    }




    



    return 0;
}


void log(string st) {
    packetLog l;
    strcpy(l.msg,st.c_str());
    s.sendUDP(&l);

}
void handle_client(int index) {
    packetS p;
    while(s.recvTCP(index, &p)) {

            log("[Server] Ricevuto messaggio dal Sensore");
            //cout<<"messaggio ricevuto: "<<p.nome<<" "<<p.valore<<endl;


    }





}