/*
 * virtual_core.h
 *
 *  Created on: May 2, 2010
 *      Author: Anh Tran
 *
 *  Define of a virtual core with only I/O ports and virtual functions without implemented
 *  All cores have to inherit this virtual core and to implement these virtual functions
 *
 *  Revised:
 *  	+ Oct 25, 2010: now support virtual-channel router
 *  	+ Nov 18, 2010: support credit-based flow control
 */

#ifndef VIRTUAL_PROC_H_
#define VIRTUAL_PROC_H_

#include <systemc.h>
//#include "asap/router_to_asapcore.h"
//#include "asap/asap_core.h"
//#include "asap/asapcore_to_router.h"
#include "../definition.h"
#include "proc_evaluation.h"

class VirtualProc: public sc_module{
  public:
	// clk and reset
	sc_in <bool> clk;
	sc_in <bool> reset;

	// Input interface
	sc_in <bool> valid_in;
	sc_in <Flit> flit_in;
//	sc_out <bool> in_full;
	sc_out <bool> in_vc_buffer_rd[MAX_N_VCS];

	// output interface
	sc_out <bool> valid_out;
	sc_out <Flit> flit_out;
//	sc_in <bool> out_full[MAX_N_VCS];	// "full" signals from virtual channels of the local router port
	sc_in <bool> out_vc_buffer_rd[MAX_N_VCS];	// "full" signals from virtual channels of the local router port


	//----------- functions
	// initialize all constants inside the processor (x,y)
	virtual void initialize(int x, int y)=0;

	// evaluation
//	virtual SyntheticFactors *synthetic_evaluation()=0;
//	virtual AsAPFactors *asap_evaluation()=0;
	virtual ProcEvaluationFactors *evaluation()=0;


	// constructor
	VirtualProc (sc_module_name name): sc_module(name){}

};

#endif /* VIRTUAL_PROC_H_ */
