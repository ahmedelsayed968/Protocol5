//
// Created by asr96 on 12/1/2022.
//

#ifndef PROTOCOL5_PHYSICALLAYER_H
#define PROTOCOL5_PHYSICALLAYER_H
#include "bits/stdc++.h"
#include "protocol.h"
using namespace std;
class PhysicalLayer {
private:
//    std::vector<>
    vector<Protocol> decvice;
public:
    PhysicalLayer();
    void add_device(Protocol &p);
    void send(Frame* frame);
};


#endif //PROTOCOL5_PHYSICALLAYER_H
