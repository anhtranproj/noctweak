/*
 * definition.h
 *
 *  Created on: Apr 12, 2010
 *      Author: Anh Tran, VCL, UC Davis
 *
 *  Definition of Flit and Packet
 */

#ifndef DEFINITION_H_
#define DEFINITION_H_

#include <systemc.h>
#include "common_parameters.h"
#include "router/router_parameters.h"

using namespace std;

struct Flit{
	bool head;
	bool tail;
	int src_x;
	int src_y;
	int dst_x;
	int dst_y;
	int payload;
	int injected_time;
	int fifo_id;
	int vc_id;	// dynamically change at its virtual-channel router
	int current_out_port;	// used by local shared buffers
	int next_out_port;	// lookahead routing
	int type;	// ACK(=1) or data(=0), using for adaptive in-order routing
	bool ACK_required;	// 1= YES, 0=NO
	int packet_length;	// contained in the head flit for packet-length-aware adaptive routing

	Flit(){
		head = 0;
		tail = 0;
		src_x = 0;
		src_y = 0;
		dst_x = 0;
		dst_y = 0;
		payload = 0;
		injected_time = -1;
		fifo_id = -1;
		vc_id = -1;
		current_out_port = -1;
		next_out_port = -1;
		type = 0;
		ACK_required = 0;
		packet_length = 1;
	}

//	Flit(bool h, bool t, int s_x, int s_y, int d_x, int d_y, int p, int inj_t, int f_id, int _vc_id);

    inline bool operator ==(const Flit &flit) const {
    	return (flit.head == head && flit.tail == tail
    			&& flit.src_x == src_x && flit.src_y == src_y
    			&& flit.dst_x == dst_x && flit.dst_y == dst_y
    			&& flit.payload == payload
    			&& flit.injected_time == injected_time
    			&& flit.fifo_id == fifo_id)
    			&& flit.vc_id == vc_id
    			&& flit.current_out_port == current_out_port
    			&& flit.next_out_port == next_out_port
				&& flit.type == type
				&& flit.ACK_required == ACK_required
				&& flit.packet_length == packet_length;
    }
};

class Packet{
  public:
//	Flit *head_flit;
	Flit *flit[MAX_PACKET_LENGTH];

	Packet(){}

//	Packet(int traffic_type, int src_x, int src_y);

//	Packet(int length, int src_x, int src_y, int dst_x, int dst_y, int dst_fifo_id, int vc_id);

	Packet(Flit *head_flit, int packet_length);

  private:
};

// Output overloading for avoiding os errors while compiled
inline ostream & operator <<(ostream & os, const Flit & flit){
  	os << "[type: ";
	switch (flit.head) {
	case 1:
	    os << "HEAD";
	    break;
	case 0:
		if (!flit.tail) os << "BODY";
		else os << "TAIL";
 	    break;
	}

	os << ", src: (" << flit.src_x << ", " << flit.src_y << ")"
			<< "dst: (" << flit.dst_x << flit.dst_y << ")"
			<< "payload: " << flit.payload
			<< "injected_time: " << flit.injected_time
			<< "fifo_id: " << flit.fifo_id
			<< "vc_id: " << flit.vc_id
			<< "current_out_port: " << flit.current_out_port
			<< "next_out_port: " << flit.next_out_port
			<< "type: " << flit.type
			<< "ACK_required: " << flit.ACK_required
			<< "packet_legnth: " << flit.packet_length;

    return os;
}

// Trace overloading for avoiding sc_trace errors while compiled
inline void sc_trace(sc_trace_file * &tf, const Flit &flit, string &name){
	sc_trace(tf, flit.head, name + ".head");
    sc_trace(tf, flit.tail, name + ".tail");
    sc_trace(tf, flit.src_x, name + ".src_x");
    sc_trace(tf, flit.src_y, name + ".src_y");
    sc_trace(tf, flit.dst_x, name + ".dst_x");
    sc_trace(tf, flit.dst_y, name + ".dst_y");
    sc_trace(tf, flit.payload, name + ".payload");
    sc_trace(tf, flit.injected_time, name + ".injected_time");
    sc_trace(tf, flit.fifo_id, name + ".fifo_id");
    sc_trace(tf, flit.vc_id, name + ".vc_id");
    sc_trace(tf, flit.current_out_port, name + ".current_out_port");
    sc_trace(tf, flit.next_out_port, name + ".next_out_port");
    sc_trace(tf, flit.type, name + ".type");
    sc_trace(tf, flit.ACK_required, name + ".ACK_required");
    sc_trace(tf, flit.packet_length, name + ".packet_length");
}

#endif /* DEFINITION_H_ */
