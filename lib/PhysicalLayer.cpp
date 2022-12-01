//
// Created by asr96 on 12/1/2022.
//
#include "PhysicalLayer.h"
#include "Frame.h"
#include "time.h"
void PhysicalLayer::add_device(Protocol &p) {
    this->decvice.push_back(p);
}

void PhysicalLayer::send(Frame* frame) {
    _sleep(10);
    decvice[frame->getDestinationId()].Physical_buffer.push(frame);
}

PhysicalLayer::PhysicalLayer() {

}
