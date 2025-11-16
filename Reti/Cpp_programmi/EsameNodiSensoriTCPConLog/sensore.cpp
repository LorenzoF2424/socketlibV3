#include "lib.h"

Socket s;
int main(int argc, char **argv) {

    //s.bindTCP("127.0.0.1",7000);
    s.connectTCP("127.0.0.1",7777);
    cout<<"test1"<<endl;
    packetS p;
    string ciao="simone";
    strcpy(p.nome,ciao.c_str());
    cout<<"test2"<<endl;

    while(true) {
        //cout<<"ciao sono il client"<<endl;
        cout<<p.nome<<" "<<p.valore<<endl;


        p.valore=rand()%100;
        if (!s.sendTCP(&p)) {
            cout<<"Errore send"<<endl;
        }

        sleep(1);
    }

    



    return 0;
}






