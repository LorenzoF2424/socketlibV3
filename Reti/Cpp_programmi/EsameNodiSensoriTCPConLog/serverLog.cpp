#include "lib.h"
Socket s;
void handle_recv() {
    ofstream F;
    F.open("server.log");
    packetLog l;
    while(true) {

        l=s.recvUDP<packetLog>();
        cout<<l.msg<<endl;
        F<<l.msg<<endl;
        //cout<<"messaggio ricevuto: "<<p.nome<<" "<<p.valore<<endl;


    }





}

int main(int argc, char **argv) {

    s.bindUDP("127.0.0.1",8080);
    int index=0;
    bool finito=false;
    thread th(handle_recv);
    
   




    


    th.join();
    return 0;
}