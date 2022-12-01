//
// Created by asr96 on 12/1/2022.
//

#include "Frame.h"

frame_kind Frame::getKind() const {
    return kind;
}

void Frame::setKind(frame_kind kind) {
    Frame::kind = kind;
}

int Frame::getSeq() const {
    return seq;
}

void Frame::setSeq(int seq) {
    Frame::seq = seq;
}

int Frame::getAck() const {
    return ack;
}

void Frame::setAck(int ack) {
    Frame::ack = ack;
}

const Packet &Frame::getInfo() const {
    return info;
}

void Frame::setInfo(const Packet &info) {
    Frame::info = info;
}

long long Frame::getTime() const {
    return start_time;
}

void Frame::setTime() {
    this->start_time = timer::get_time();
}

long long int Frame::getEndTime() const {
    return end_time;
}

void Frame::setEndTime() {
    end_time = timer::get_time();
}

int Frame::getId() const {
    return id;
}

void Frame::setId(int id) {
    Frame::id = id;
}

int Frame::getSourceId() const {
    return sourceId;
}

void Frame::setSourceId(int sourceId) {
    Frame::sourceId = sourceId;
}

int Frame::getDestinationId() const {
    return destinationId;
}

void Frame::setDestinationId(int destinationId) {
    Frame::destinationId = destinationId;
}
