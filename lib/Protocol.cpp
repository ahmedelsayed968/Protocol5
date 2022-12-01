//
// Created by asr96 on 12/1/2022.
//

#include "Protocol.h"


boolean no_nak= True;				/* no nak has been sent yet */
seq_nr oldest_frame = MAX_SEQ + 1;

static boolean between(seq_nr a, seq_nr b, seq_nr c) {
    bool check = ((a <= b && b < c) || (c < a && a <= b) || (b < c && c < a));
    boolean ret = check?True:False;
    return ret;
}

void Protocol::send_frame(frame_kind fk, seq_nr frame_nr, seq_nr frame_expected, packet buffer[]) {
    /* construct and send a data, ack or nak frame */

    Frame s; 						/* scratch variable */
    s.kind = fk;					/* kind of frame: data|ack|nak */

    if (fk == data)
        s.info = buffer[frame_nr % NR_BUFS];

    s.seq = frame_nr;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    if (fk == nak)
        no_nak = False;

    to_physical_layer(&s);			/* transmit the frame */

    if (fk == data)
        start_timer(frame_nr % NR_BUFS);

    stop_ack_timer();				/* no need for separate ack frame */
}

// ToDo
/* wait for an event to happen; return its type of event */
void Protocol::wait_for_event(event_type *event){
    while(true){
        if(network_layer_status && !Network_layer_packets.empty())
            current_event = network_layer_ready,break;

        else if(!check_time_out())
            current_event = timeout,break;

        else if(!Physical_buffer.empty())
            current_event = frame_arrival,break;

        else
            current_event = noEvent;
    }
}

/* fetch a packet from the network layer for transmission */
void Protocol::from_network_layer(int destinationId){
    // take the packet from the network layer and send it to the physical layer
    // **this the input of our program**

    // take this packet and add to it the frame_kind, seq_nr (seq,ack)
    // set event to be network layer ready to transmit the frame to physical layer
    // then send it to the physical layer by assign it to the packet pointer
    //
    Packet *toSend = Network_layer_packets.front();
    Network_layer_packets.pop();
    Frame* frame_to_send;
    frame_to_send->setInfo(*toSend);
    frame_to_send->setAck(num_Ack);
    frame_to_send->setSeq(num_seq);
    frame_to_send->setKind(data);
    frame_to_send->setId(num_seq);
    frame_to_send->setSourceId(this->Id);
    frame_to_send->setDestinationId(destinationId);
    to_physical_layer(frame_to_send);
    inc(num_seq);
    inc(num_Ack);

}

/* deliver information from an inbound frame to the network layer */
void Protocol::to_network_layer(Packet *p){
    // get the packe
}

/* get an inbound frame from the physical layer */
void Protocol::from_physical_layer(Frame *r){
    // should take incoming frames
    // send ack to the source
    // move the frame to the Network layer

}

/* pass the frame to the physical layer */
void Protocol::to_physical_layer(Frame *frame){
    common_physical_layer.send(*frame)
    start_timer(*frame)
}



/* start the clock and enable the timeout event */
void Protocol::start_timer(Frame &frame){
    frame.setTime();
    timers.push_back(frame.getTime());
}

/* stop the clock and disable the timeout event */
void Protocol::stop_timer(Frame* frame){
    int index = frame->getId();
    this->timers[index]  = 0;
    frame->setEndTime();
}

/* start an auxiliary timer and enable the ack_timeout event */
void Protocol::start_ack_timer(seq_nr k){}

/* stop an auxiliary timer and disable the ack_timeout event */
void Protocol::stop_ack_timer(seq_nr k){}

/* allow the network to cause a network_layer_ready event */
void Protocol::enable_network_layer(){
    this->network_layer_status = true;
}

/* forbid the network to cause a network_layer_ready event */
void Protocol::disable_network_layer(){
    this->network_layer_status = false;
}
// -------------------------------------------------------------------------------------

void protocol5() {
    seq_nr ack_expected;			/* lower edge of sender's window */
    seq_nr next_frame_to_send;		/* upper edge + 1 */

    seq_nr frame_expected;			/* lower edge of receiver's window */
    seq_nr too_far;					/* upper edge + 1 */

    int i;							/* buffer pool index */
    Frame r;						/* scratch variable */
    packet out_buff[NR_BUFS];		/* outbound stream buffers */
    packet in_buff[NR_BUFS];		/* inbound stream buffers */
    boolean arrived[NR_BUFS];		/* inbound bit map */
    seq_nr nbuffered;				/* output buffers in use */
    event_type event;

    enable_network_layer();			/* init */
    ack_expected = 0;				/* ack expected on inbound stream */
    next_frame_to_send = 0;			/* the outgoing frame */
    frame_expected = 0;

    too_far = NR_BUFS;
    nbuffered = 0;					/* no buffers in use */

    for (i = 0; i < NR_BUFS; i++)
        arrived[i] = False;


    while (true) {
        wait_for_event(&event);		/* 5 different events */

        switch(event) {

            /* accept, save and transmit a new frame */
            case network_layer_ready:
                nbuffered++; 		/* expand the window */
                from_network_layer(
                        &out_buff[next_frame_to_send % NR_BUFS]);
                /* fetch new packet */

                send_frame(
                        data,
                        next_frame_to_send,
                        frame_expected,
                        out_buff);		/* transmit the frame */

                inc(next_frame_to_send); /* advance upper window edge */
                break;

                /* a data/control frame arrived */
            case frame_arrival:
                from_physical_layer(&r); /* fetch frame */

                /* data frame OK */
                if(r.kind == data) {
                    if (r.seq != frame_expected && no_nak)
                        send_frame(nak, 0, frame_expected, out_buff);
                    else
                        start_ack_timer(r.ack);

                    if (
                            between(frame_expected, r.seq, too_far)
                            && (arrived[r.seq % NR_BUFS] == false)
                            ) {
                        /* frames accepted in any order */
                        arrived[r.seq % NR_BUFS] = True;  /* buffer marked full */
                        in_buff[r.seq % NR_BUFS] = r.info; /* insert data into buffer */


                        while(arrived[frame_expected % NR_BUFS]) {
                            /* pass frames and advance window */
                            to_network_layer(&in_buff[frame_expected% NR_BUFS]);
                            no_nak = True;
                            arrived[frame_expected % NR_BUFS] = False;

                            inc(frame_expected);	/* advance lower edge of receiver */
                            inc(too_far);			/* advance upper edge of receiver */
                            start_ack_timer();		/* check if separated ack is needed */

                        }
                    }
                }

                if((r.kind == nak) &&
                   between(ack_expected, (r.ack+1) % (MAX_SEQ + 1), next_frame_to_send ))
                {
                    send_frame(data, (r.ack+1) % (MAX_SEQ + 1), frame_expected, out_buff);

                    while (between(ack_expected, r.ack, next_frame_to_send))
                    {
                        nbuffered--;				/* handle piggyback ack */
                        stop_timer(ack_expected % NR_BUFS); /* frame arrived OK */
                        inc(ack_expected);			/* advance lower edge of sender */
                    }
                }

                break;


                /* damaged frame */
            case cksum_err:
                if (no_nak)
                    send_frame(nak, 0, frame_expected, out_buff);
                break;

                /* we timed out */
            case timeout:
                send_frame(data, oldest_frame, frame_expected, out_buff);
                break;

                /* ack timer expired; resend ack */
            case ack_timeout:
                send_frame(ack, 0, frame_expected, out_buff);
                break;

        }

        if (nbuffered < NR_BUFS)
            enable_network_layer();
        else
            disable_network_layer();

    }
}

Protocol::Protocol(PhysicalLayer &physicalLayer) {
    this->common_physical_layer = physicalLayer;
    this-> Id = count;
    count++;
    this->num_Ack = 1;
    this->num_seq = 1;
}

void Protocol::send_message(string& message) {
    Packet *p;
    int size_msg = message.size();
    for(int i = 0 ;i<size_msg;i++)
        p->data[i]=message[i];
    Network_layer_packets.push(p);
}

int Protocol::check_time_out() {

   int size_timers = timers.size();
   long long time_now = timer::get_time();
   for(int i = 0;i<size_timers;i++)
   {
       long long t = time_now - timers[i];
       if(t > MAX_interval)
           return i;
   }
   return -1;

}

