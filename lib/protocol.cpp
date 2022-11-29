#include "protocol.h"
boolean no_nak= True;				/* no nak has been sent yet */
seq_nr oldest_frame = MAX_SEQ + 1;

static boolean between(seq_nr a, seq_nr b, seq_nr c) {
    bool check = ((a <= b && b < c) || (c < a && a <= b) || (b < c && c < a));
    boolean ret = check?True:False;
	return ret;
}

static void send_frame(frame_kind fk, seq_nr frame_nr, seq_nr frame_expected, packet buffer[]) {
	/* construct and send a data, ack or nak frame */
	
	frame s; 						/* scratch variable */
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
void wait_for_event(event_type *event){}

/* fetch a packet from the network layer for transmission */
void from_network_layer(packet *p){}

/* deliver information from an inbound frame to the network layer */
void to_network_layer(packet *p){}

/* get an inbound frame from the physical layer */
void from_physical_layer(frame *r){}

/* pass the frame to the physical layer */
void to_physical_layer(frame *s){}

/* start the clock and enable the timeout event */
void start_timer(seq_nr k){}

/* stop the clock and disable the timeout event */
void stop_timer(seq_nr k){}

/* start an auxiliary timer and enable the ack_timeout event */
void start_ack_timer(seq_nr k){}

/* stop an auxiliary timer and disable the ack_timeout event */
void stop_ack_timer(seq_nr k){}

/* allow the network to cause a network_layer_ready event */
void enable_network_layer(void){}

/* forbid the network to cause a network_layer_ready event */
void disable_network_layer(void){}
// -------------------------------------------------------------------------------------

void protocol5() {
	seq_nr ack_expected;			/* lower edge of sender's window */
	seq_nr next_frame_to_send;		/* upper edge + 1 */
	
	seq_nr frame_expected;			/* lower edge of receiver's window */
	seq_nr too_far;					/* upper edge + 1 */
	
	int i;							/* buffer pool index */
	frame r;						/* scratch variable */
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
						start_ack_timer();
					
					if (
						between(frame_expected, r.seq, too_far)
						&& (arrived[r.seq % NR_BUFS] == false)
					) {
						/* frames accepted in any order */
						arrived[r.seq % NR_BUFS] = true;  /* buffer marked full */
						in_buff[r.seq % NR_BUFS] = r.info; /* insert data into buffer */
						
						
						while(arrived[frame_expected % NR_BUFS]) {
							/* pass frames and advance window */
							to_network_layer(&in_buff[frame_expected% NR_BUFS]);
							no_nak = true;
							arrived[frame_expected % NR_BUFS] = false;
							
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