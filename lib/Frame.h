//
// Created by asr96 on 12/1/2022.
//

#ifndef PROTOCOL5_FRAME_H
#define PROTOCOL5_FRAME_H
#define MAX_PKT_SIZE 1024
#include <chrono>
#include "timer.h"
typedef struct {
    unsigned char data[MAX_PKT_SIZE];
} Packet;
typedef enum { data, ack, nak } frame_kind;
class Frame {
    frame_kind kind;                    /* what kind of frame? */
    int seq;                         /* sequence number */
    int ack;                         /* ACK number */
    Packet info;
    long long start_time;
    long long end_time;
    int id;
    int sourceId;
    int destinationId;
public:
    frame_kind getKind() const;

    void setKind(frame_kind kind);

    long long int getEndTime() const;

    void setEndTime();

    int getId() const;

    void setId(int id);

    int getSeq() const;

    void setSeq(int seq);

    int getAck() const;

    void setAck(int ack);

    const Packet &getInfo() const;

    long long getTime() const;

    void setTime();

    void setInfo(const Packet &info);

    int getSourceId() const;

    void setSourceId(int sourceId);

    int getDestinationId() const;

    void setDestinationId(int destinationId);
};


#endif //PROTOCOL5_FRAME_H
