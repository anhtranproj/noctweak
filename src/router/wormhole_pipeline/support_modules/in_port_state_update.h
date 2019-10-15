/*
 * in_port_state_update.h
 *
 *  Created on: Nov 22, 2010
 *      Author: Anh Tran
 *
 *  Input port states
 *
 */

#ifndef IN_PORT_STATE_UPDATE_H_
#define IN_PORT_STATE_UPDATE_H_

#include "systemc.h"
#include "../../router_parameters.h"

SC_MODULE (InPortStateUpdate){
	int in_port;	// input port identification

	sc_in <bool> empty;
	sc_in <bool> tail_out_reg[N_ROUTER_PORTS];
	sc_in <int> sa_allocated_reg;	// = [po]: which output port has been reserved for this input buffer
	sc_in <int> in_port_state_reg;

	sc_out <int> in_port_state;

	void in_port_state_process(){
			switch (in_port_state_reg.read()){
				case (IDLE):
						if (!empty.read())	// having at least a flit
							in_port_state.write(WAIT_FOR_GRANT);	// assumed finishing RC
						else
							in_port_state.write(IDLE);
					break;
				case (WAIT_FOR_GRANT):
						if (sa_allocated_reg.read() >= 0){	// is allocated for an out port po
							int po = sa_allocated_reg.read();
							if (tail_out_reg[po].read()){
								if (!empty.read())
									in_port_state.write(WAIT_FOR_GRANT);
								else
									in_port_state.write(IDLE);
							}
							else {
								in_port_state.write(po);	// record this output VC
							}
						}
						else{	// not allocated yet
							in_port_state.write(WAIT_FOR_GRANT);
						}
					break;
				default: // po >=0: if was already allocated
						int po = in_port_state_reg.read();
						if (tail_out_reg[po].read()){
							if (!empty.read())
								in_port_state.write(WAIT_FOR_GRANT);
							else
								in_port_state.write(IDLE);
						}
						else{
							in_port_state.write(po);
						}
					break;
			}
	}

	SC_CTOR (InPortStateUpdate){
		SC_METHOD (in_port_state_process);
		sensitive << empty << sa_allocated_reg;
		for (int po=0; po<N_ROUTER_PORTS; po++){
			sensitive << tail_out_reg[po];
		}
		sensitive << in_port_state_reg;
	}
};


#endif /* IN_PORT_STATE_UPDATE_H_ */
