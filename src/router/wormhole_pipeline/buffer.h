/*
 * buffer.h
 *
 *  Created on: Apr 13, 2010
 *      Author: Anh Tran, VCL, UC Davis
 *
 *  Model of buffer using C++ queue class with full/empty signal
 *  Read/write at positive clock edges
 *
 *	Modified:
 *		+ 2010.10.25: add initialize() function for setup buffer size and reserve space
 *		+ 2010.11.18: don't send full signal; used for credit-based flow control
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <queue>
#include <systemc.h>
//#include "parameters.h"
#include "../../definition.h"
#include "../router_parameters.h"
#include "../router_activities.h"

using namespace std;

SC_MODULE(Buffer){
	// reset and clock
	sc_in <bool> clk;
	sc_in <bool> reset;

	// write side
	sc_in <bool> valid_in;
	sc_in <Flit> buffer_in;
//	sc_out <bool> full;

	// read side
	sc_in <bool> rd_req;
	sc_out <Flit> buffer_out;
	sc_out <bool> empty;

	// for energy estimation
	BufferActivities *getBufferActivities();

	// process
	void buffer_process();	// write

	SC_CTOR(Buffer){
		SC_METHOD(buffer_process);
		sensitive << reset.pos() << clk.pos();

	}

public:
	// set buffer_size, buffer_reserve
	void initilize(unsigned int _buffer_size);

//	void initilize(unsigned int _buffer_size, int _buffer_th_off);

//	void initilize(unsigned int _buffer_size, int _buffer_th_off, int _buffer_th_on);


//	int buffer_th_off;	// assert full when the idle number of entries <= Toff
//	int buffer_th_on;	// assert full when the idle number of entries >= Ton

	// functions
private:
	queue <Flit> buffer;	// a C++ built-in queue
	unsigned int buffer_size;	// number of flits

//	bool is_full();	// whether the buffer is full
//	bool is_off();

	// for energy and power computation
	int n_rd_wr_cycles;
	int n_rd_only_cycles;
	int n_wr_only_cycles;
	int n_inactive_cycles;
};

#endif /* BUFFER_H_ */
