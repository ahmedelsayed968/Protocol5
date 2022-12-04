//
// Created by asr96 on 12/3/2022.
//

#ifndef PROTOCOL5_PHYSICAL_LAYER_H
#define PROTOCOL5_PHYSICAL_LAYER_H

#include "bits/stdc++.h"
#include "Protocol.h"
#include "Frame.h"
using namespace std;
class Protocol;
class Frame;
class Physical_Layer {
private:
//    std::vector<>
    vector<Protocol> decvice;
public:
    Physical_Layer();
    void add_device(Protocol &p);
    void send(Frame& frame);
    friend class Frame;
};



#endif //PROTOCOL5_PHYSICAL_LAYER_H
