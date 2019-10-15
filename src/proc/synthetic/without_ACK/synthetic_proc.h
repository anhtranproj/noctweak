/*
 * pe.h
 *
 *  Created on: Apr 16, 2010
 *      Author: Anh Tran, VCL, UC Davis
 *
 *  Model of a processing element (PE)
 *  for injecting and consuming packet into/from the network
 *
 *  Revised:
 *  	+ Oct 25, 2010: now supports virtual-channel router
 *  	+ 2010.11.18: support credit-based flow control
 *  	+ 2010.12.06: add tornado traffic
 */

#ifndef SYNTHETIC_PROC_H_
#define SYNTHETIC_PROC_H_

//#include <math.h>
#include <queue>
#include <systemc.h>
#include "../../../definition.h"
#include "../../virtual_proc.h"
#include "../../../common_functions.h"
#include "../../../router/router_parameters.h"
#include "../../proc_parameters.h"
#include "../synthetic_parameters.h"
//#include "../../out_vc_arbiter.h"

#include "../../../global_variables.h"

#include "../../proc_functions.h"
#include "../../proc_evaluation.h"

using namespace std;

class SyntheticProc: public VirtualProc{
  public:

	// local address
	int local_x;
	int local_y;
/*
	// clk and reset
	sc_in <bool> clk;
	sc_in <bool> reset;

	// Input interface
	sc_in <bool> valid_in;
	sc_in <Flit> flit_in;
	sc_out <bool> in_full;

	// output interface
	sc_out <bool> valid_out;
	sc_out <Flit> flit_out;
	sc_in <bool> out_full;
*/

	// functions
	void initialize(int x, int y);

	// for evaluation
	ProcEvaluationFactors *evaluation();

//	OutVCArbiter *vc_arbiter;	// choose VC of the local router port

	// signals
//	sc_signal <bool> tail_reg;	// for supporting vc_arbiter
//	sc_signal <int> out_vc_grant;

	sc_signal <int> out_vc_remain[MAX_N_VCs];		// keep trace of number of idle entries of each output VC
	sc_signal <int> out_vc_remain_reg[MAX_N_VCs];

	sc_signal <bool> count_plus[MAX_N_VCs];	// = out_vc_buffer_rd
	sc_signal <bool> count_minus[MAX_N_VCs];

	sc_signal <bool> queue_out_valid;	// queue output before sent to LOCAL port
	sc_signal <Flit> queue_out;

	// processes
	void tx_process();

	void flit_out_process();

	void out_vc_remain_process();	// update out_vc_remain

	void count_plus_process();	// pipelined out_vc_remain
	void out_vc_remain_reg_process();	// pipelined out_vc_remain

	void rx_process();

	// constructor with process enable
	SC_HAS_PROCESS(SyntheticProc);
	SyntheticProc (sc_module_name name): VirtualProc(name){
		// send flit
		SC_METHOD (tx_process);
		sensitive << reset.pos() << clk.pos();

		// receive flit
		SC_METHOD (rx_process);
		sensitive << reset.pos() << clk.pos();

/*
		// vc_arbiter
		vc_arbiter = new OutVCArbiter("Out_Local_VC_Arbiter");
		vc_arbiter->clk(clk);
		vc_arbiter->reset(reset);
		vc_arbiter->tail_reg(tail_reg);
		for (int k=0; k<MAX_N_VCS; k++){
			vc_arbiter->out_vc_remain[k](out_vc_remain[k]);
//			vc_arbiter->out_vc_remain_reg[k](out_vc_remain_reg[k]);
//			vc_arbiter->count_minus[k](count_minus[k]);
//			vc_arbiter->count_plus[k](count_plus[k]);
		}
		vc_arbiter->grant(out_vc_grant);
*/

		// update out_vc_remain
		SC_METHOD (out_vc_remain_process);
		for (int vo=0; vo<RouterParameter::n_VCs; vo++){
			sensitive << out_vc_remain_reg[vo];
			sensitive << count_plus[vo];
//			sensitive << out_vc_buffer_rd[vo];
			sensitive << count_minus[vo];
		}

		// pipelined out_vc_remain
		SC_METHOD (out_vc_remain_reg_process);
		sensitive << clk.pos() << reset.pos();

		// count_plus = out_vc_buffer
		SC_METHOD (count_plus_process);
		for (int vo=0; vo<RouterParameter::n_VCs; vo++){
			sensitive << out_vc_buffer_rd[vo];
//			sensitive << out_port_EFC_en;
		}

		// flit_out
		SC_METHOD (flit_out_process);
		sensitive << queue_out_valid << queue_out << out_vc_remain[0];
	}

	// functions
  private:
	bool was_head[MAX_N_VCS];	// whether a packet head flit was received on a VC
	int injected_packet_count;
	int received_packet_count;
	double total_latency;
	double max_latency;
	double min_latency;

	// packet inter-injection time (cycles) with exponential distribution
//	int inter_injection_time(double flit_rate, int type);

	int next_injection_time;	// time for injecting next packet

	// a "infinite" queue of injected flits at the source PE
	queue <Flit> source_queue;

//	// where the source queue is full;
//	bool is_source_queue_full();

	// time that the head flit was injected
	int head_injected_time;

	// time that the tail flit of the same packet is receiving
	int tail_receiving_time;

	// generate a head flit with random destination
//	Flit *create_head_flit_random(int _src_x, int _src_y, int _time);
	// generate a head flit with transpose destination
//	Flit *create_head_flit_transpose(int _src_x, int _src_y, int _time);
	// generate a head flit with bit-reversal destination
//	Flit *create_head_flit_bit_complement(int _src_x, int _src_y, int _time);

//	Flit *create_head_flit_tornado(int _src_x, int _src_y, int _time);

//	Flit *create_head_flit_ACK(int _src_x, int _src_y, int _dst_x, int _dst_y, int _time);
};

#endif /* SYNTHETIC_PROC_H_ */
