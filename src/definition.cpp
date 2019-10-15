/*
 * definition.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: Anh Tran, VCL, UC Davis
 */

#include "definition.h"

/*
Flit::Flit(bool h, bool t, int s_x, int s_y, int d_x, int d_y, int p, int inj_t, int f_id, int _vc_id){
	head = h;
	tail = t;
	src_x = s_x;
	src_y = s_y;
	dst_x = d_x;
	dst_y = d_y;
	payload = p;
	injected_time = inj_t;
	fifo_id = f_id;
	vc_id = _vc_id;
}
*/

Packet::Packet(Flit *head_flit, int packet_length){
	// head flits
	flit[0] = head_flit;
	flit[0]->packet_length = packet_length;

	if (packet_length > 1){
		int head_flit_inj_time = head_flit->injected_time;

		// create body flits
		for (int i=1; i<packet_length-1; i++){
			flit[i] = new Flit();
			flit[i]->head = 0;
			flit[i]->tail = 0;
			flit[i]->payload = rand();
			//		flit[i]->fifo_id = 0;
			flit[i]->injected_time = head_flit_inj_time + i;
			//		flit[i]->vc_id = 0;
		}

		// create tail flit
		flit[packet_length-1] = new Flit();
		flit[packet_length-1]->head = 0;
		flit[packet_length-1]->tail = 1;
		flit[packet_length-1]->payload = rand();
		//	flit[packet_length-1]->fifo_id = 0;
		flit[packet_length-1]->injected_time = head_flit_inj_time + packet_length-1;
		//	flit[packet_length-1]->vc_id = 0;
	}
	else {	// packet_length == 1 -> head is also tail
		flit[0]->tail = 1;
	}
}
