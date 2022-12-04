//
// Created by asr96 on 12/1/2022.
//

#ifndef PROTOCOL5_PROTOCOL_H
#define PROTOCOL5_PROTOCOL_H
typedef unsigned int seq_nr;
#define MAX_PKT 1024                    /* packet size in bytes */
#include "Frame.h"
#include <chrono>
#include "timer.h"
#include "Physical_Layer.h"
#include "bits/stdc++.h"
#include "queue"
#define MAX_interval 3000
typedef enum { False, True } boolean;   /* boolean type */
//#define MAX_SEQ 7					/* should be 2*n-1 */
#define NR_BUFS ((MAX_SEQ+1)/2)
/* macro inc */
//#define inc(k)
class Physical_Layer;
class Frame;
using namespace std;

typedef enum {
    frame_arrival,
    timeout,
    network_layer_ready,
    noEvent
} event_type;



class Protocol{
    int num_Ack;
    int num_seq;
    int Id;
    static int countP;
    queue<Packet*> Network_layer_packets;
    Physical_Layer* common_physical_layer;
    bool network_layer_status;
    event_type current_event;
    vector<long long> timers;
    int check_time_out();
    void send_message(string& message);
    void wait_for_event(event_type& event);
    void from_network_layer(int destinationId);
    void enable_network_layer();
    void disable_network_layer();
    void start_timer(Frame& frame);
    void stop_timer(Frame& frame);
    void to_physical_layer(Frame& frame);
    void display_frame(Frame& frame);
    void to_network_layer(Frame& frame);
    void from_physical_layer();
    void inc(int&k);
public:
    queue<Frame> Physical_buffer;
    Protocol(Physical_Layer& physicalLayer);

    void start_ack_timer(seq_nr k);
    void stop_ack_timer(seq_nr k);
    friend class Physical_Layer;
    static boolean between(seq_nr a, seq_nr b, seq_nr c);
    void protocol5();
};

int Protocol::countP =0;

#endif //PROTOCOL5_PROTOCOL_H
