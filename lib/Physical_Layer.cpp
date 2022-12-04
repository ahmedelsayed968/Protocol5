//
// Created by asr96 on 12/3/2022.
//

#include "Physical_Layer.h"
void Physical_Layer::add_device(Protocol &p) {
    this->decvice.push_back(p);
}

void Physical_Layer::send(Frame& frame) {
    this_thread::sleep_for(chrono::milliseconds(500));
    decvice[(frame).getDestinationId()].Physical_buffer.push(frame);
}

Physical_Layer::Physical_Layer() {

}