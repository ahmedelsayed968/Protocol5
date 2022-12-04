#include <iostream>
#include "lib/Physical_Layer.h"
//class Physical_Layer;
#include "lib/Protocol.h"
#include "bits/stdc++.h"
#include "lib/Frame.h"
using namespace std;
int main() {
    Physical_Layer physicalLayer;

    Protocol* sender = new Protocol(physicalLayer);
    Protocol* receiver = new Protocol(physicalLayer);
    while(true){
        string msg;
        cout << "Enter Message to Send or -1 to Disconnect" <<endl;
        cin >> msg;
        if(msg !="-1"){
            Packet p;
            for (int it = 0; i < msg.size(); i++)
                p.data[i] = msg[i];
            Frame f;
            f.setInfo(p);
            sender->Physical_buffer.push(f);
        }
        else{
            cout << "Disconnected!" <<endl;
            break;
        }


    }



    return 0;
}
