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
#include "PhysicalLayer.h"
#define MAX_interval 3000
typedef enum { False, True } boolean;   /* boolean type */
//#define MAX_SEQ 7					/* should be 2*n-1 */
#define NR_BUFS ((MAX_SEQ+1)/2)
/* macro inc */
#define inc(k) if (k < MAX_SEQ) k = k + 1; else k = 0
using namespace std;

typedef enum {
    frame_arrival,
    timeout,
    network_layer_ready,
    noEvent
} event_type;

class PhysicalLayer;

class Protocol{
    int num_Ack;
    int num_seq;
    int Id;
    static int count;
    queue<Packet*> Network_layer_packets;
    PhysicalLayer common_physical_layer;
    bool network_layer_status;
    event_type current_event;
    vector<long long> timers;
    int check_time_out();
    void send_message(string& message);
    void wait_for_event(event_type *event);
    void from_network_layer(int destinationId);
    void enable_network_layer(void);
    void disable_network_layer(void);
    void start_timer(Frame &frame);
    void stop_timer(Frame* frame);
    void to_physical_layer(Frame *frame);
    void display_frame(Frame* frame);
    void to_network_layer(Frame *frame);
    void from_physical_layer(Frame *r);
public:
    queue<Frame*> Physical_buffer;
    Protocol(PhysicalLayer& physicalLayer);

    /* wait for an event to happen; return its type of event */

/* fetch a packet from the network layer for transmission */

/* deliver information from an inbound frame to the network layer */

/* get an inbound frame from the physical layer */

/* pass the frame to the physical layer */

/* start the clock and enable the timeout event */

/* stop the clock and disable the timeout event */

/* start an auxiliary timer and enable the ack_timeout event */
    void start_ack_timer(seq_nr k);

/* stop an auxiliary timer and disable the ack_timeout event */
    void stop_ack_timer(seq_nr k);


//    void send_frame(frame_kind fk, seq_nr frame_nr, seq_nr frame_expected, packet buffer[]);
    static boolean between(seq_nr a, seq_nr b, seq_nr c);
    void protocol5();
};

int Protocol::count =0;

#endif //PROTOCOL5_PROTOCOL_H
