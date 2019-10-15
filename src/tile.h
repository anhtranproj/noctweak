/*
 * tile.h
 *
 *  Created on: May 2, 2010
 *      Author: Anh Tran
 *
 *  Tile = Proc + Router
 *
 *  Revised:
 *  	+ 2010.11.18:
 *  		- support credit-based flow control
 */

#ifndef TILE_H_
#define TILE_H_

#include "proc/virtual_proc.h"
#include "proc/synthetic/without_ACK/synthetic_proc.h"
#include "proc/synthetic/with_ACK/synthetic_with_ACK_proc.h"
#include "proc/embedded/without_ACK/embedded_proc.h"
#include "proc/embedded/with_ACK/embedded_with_ACK_proc.h"
//#include "proc/asap/asap_proc.h"
#include "router/virtual_router.h"
//#include "router/wormhole/wormhole_router.h"
#include "router/wormhole_pipeline/wormhole_pipeline.h"


SC_MODULE (Tile){

	// local address
//	sc_in <int> local_x;
//	sc_in <int> local_y;

	sc_in <bool> clk;
	sc_in <bool> reset;

	// input ports
	sc_in <bool> valid_in[ROUTER_PORTS-1];
	sc_in <Flit> flit_in[ROUTER_PORTS-1];
//	sc_out <bool> in_full[ROUTER_PORTS-1][MAX_N_VCS];
	sc_out <bool> in_vc_buffer_rd[ROUTER_PORTS-1][MAX_N_VCS];

	// output ports
	sc_out <bool> valid_out[ROUTER_PORTS-1];
	sc_out <Flit> flit_out[ROUTER_PORTS-1];
//	sc_in <bool> out_full[ROUTER_PORTS-1][MAX_N_VCS];
	sc_in <bool> out_vc_buffer_rd[ROUTER_PORTS-1][MAX_N_VCS];

	// signals and variables
	sc_signal <bool> tx_valid_out;
	sc_signal <Flit> tx_flit_out;
//	sc_signal <bool> tx_out_full[MAX_N_VCS];
	sc_signal <bool> tx_out_vc_buffer_rd[MAX_N_VCS];

	sc_signal <bool> rx_valid_in;
	sc_signal <Flit> rx_flit_in;
//	sc_signal <bool> rx_in_full[MAX_N_VCS];
	sc_signal <bool> rx_in_vc_buffer_rd[MAX_N_VCS];	// only consider buffer_rd[0]; is processor read an input flit?

	// setup don't-care signals
	void initialize(){
		// just one buffer at the core connecting to the LOCAL output port
//		for (int k=1; k<MAX_N_VCS; k++)
//			rx_in_buffer_rd[k].write(0);	// dont care, dont read
	}

	// modules
//	PE *synth_pe;
//	AsAPProc *asap_proc;
	VirtualProc *proc;

	VirtualRouter *router;

	SC_CTOR (Tile){
		switch (CommonParameter::platform_type){
			case (PLATFORM_SYNTHETIC):
				switch (ProcessorParameters::packet_delivery_type){
					case DELIVERY_WITHOUT_ACK:
//						cout << "Binding Synthetic Proc ..." << endl;
						proc = new SyntheticProc("Synthetic_Proc");
						break;
					case DELIVERY_WITH_ACK:
						proc = new SyntheticWithACKProc("Synthetic_With_ACK_Proc");
						break;
					default:;
				}
				break;
			case (PLATFORM_ASAP):
//				cout << "Binding AsAP Proc ..." << endl;
//				proc = new AsAPProc("AsAP_Proc");
/*
				asap_proc->clk(clk);
				asap_proc->reset(reset);
				asap_proc->valid_out(tx_valid_out);
				asap_proc->flit_out(tx_flit_out);
				asap_proc->out_full(tx_out_full);
				asap_proc->valid_in(rx_valid_in);
				asap_proc->flit_in(rx_flit_in);
				asap_proc->in_full(rx_in_full);
*/
				break;
			case (PLATFORM_EMBEDDED):
					switch (ProcessorParameters::packet_delivery_type){
						case DELIVERY_WITHOUT_ACK:
//							cout << "Binding Embedded Proc ..." << endl;
							proc = new EmbeddedProc("Embedded_Proc");
							break;
						case DELIVERY_WITH_ACK:
							proc = new EmbeddedWithACKProc("Synthetic_With_ACK_Proc");
							break;
						default:;
					}
					break;
			default:
				cout << "ERROR: Invalid Platform Type!" << endl;
				exit (0);
		}
		proc->clk(clk);
		proc->reset(reset);
		proc->valid_out(tx_valid_out);
		proc->flit_out(tx_flit_out);
		for (int k=0; k<MAX_N_VCS; k++){
			proc->out_vc_buffer_rd[k](tx_out_vc_buffer_rd[k]);
		}
		proc->valid_in(rx_valid_in);
		proc->flit_in(rx_flit_in);
		for (int k=0; k<MAX_N_VCS; k++){
			proc->in_vc_buffer_rd[k](rx_in_vc_buffer_rd[k]);
		}

		switch (RouterParameter::router_type){
			case (ROUTER_WORMHOLE):
//				cout << "Binding Wormhole Router ..." << endl;
//				router = new WormholeRouter("Wormhole_Router");
/*
				WormholeRouter *wh_router = (WormholeRouter*) router;

				wh_router->clk(clk);
				wh_router->reset(reset);
				wh_router->valid_in[LOCAL](tx_valid_out);
				wh_router->flit_in[LOCAL](tx_flit_out);
				wh_router->in_full[LOCAL](tx_out_full[0]);

				wh_router->valid_out[LOCAL](rx_valid_in);
				wh_router->flit_out[LOCAL](rx_flit_in);
				wh_router->out_full[LOCAL](rx_in_full[0]);

				for (int i=0; i<ROUTER_PORTS-1; i++){
					wh_router->valid_in[i](valid_in[i]);
					wh_router->flit_in[i](flit_in[i]);
					wh_router->in_full[i](in_full[i][0]);

					wh_router->valid_out[i](valid_out[i]);
					wh_router->flit_out[i](flit_out[i]);
					wh_router->out_full[i](out_full[i][0]);
				}
*/
				break;
			case (ROUTER_VC):
/*
				switch (RouterParameter::n_VCs){
					case (1):
						cout << "Binding Virtual Channel Router (1 VC per port)..." << endl;
						router = new VCRouter<1>("Virtual_Channel_Router_1_VC");
						break;
					case (2):
						cout << "Binding Virtual Channel Router (2 VCs per port)..." << endl;
						router = new VCRouter<2>("Virtual_Channel_Router_2_VCs");
						break;
					case (4):
						cout << "Binding Virtual Channel Router (4 VCs per port)..." << endl;
						router = new VCRouter<4>("Virtual_Channel_Router_4_VCs");
						break;
					case (8):
						cout << "Binding Virtual Channel Router (8 VCs per port)..." << endl;
						router = new VCRouter<8>("Virtual_Channel_Router_8_VCs");
						break;
				}
*/
				break;
			case (ROUTER_SB_1IB):
/*
				switch (RouterParameter::n_SBs){
					case (0):
//						cout << "Binding Shared Buffer Router 1IB with 0 Shared Buffers (Wormhole)..." << endl;
						router = new SBR_1IB_Router<0>("Shared_Buffer_Router_1IB_0SBs");
						break;

					case (5):
//						cout << "Binding Shared Buffer Router 1IB with 5 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<5>("Shared_Buffer_Router_1IB_5SBs");
						break;

					case (6):
//						cout << "Binding Shared Buffer Router 1IB with 6 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<6>("Shared_Buffer_Router_1IB_6SBs");
						break;

					case (7):
//						cout << "Binding Shared Buffer Router 1IB with 7 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<7>("Shared_Buffer_Router_1IB_7SBs");
						break;

					case (8):
//						cout << "Binding Shared Buffer Router 1IB with 8 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<8>("Shared_Buffer_Router_1IB_8SBs");
						break;

					case (9):
//						cout << "Binding Shared Buffer Router 1IB with 9 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<9>("Shared_Buffer_Router_1IB_9SBs");
						break;

					case (10):
//						cout << "Binding Shared Buffer Router 1IB with 10 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<10>("Shared_Buffer_Router_1IB_10SBs");
						break;

					case (12):
//						cout << "Binding Shared Buffer Router 1IB with 12 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<12>("Shared_Buffer_Router_1IB_12SBs");
						break;

					case (15):
//						cout << "Binding Shared Buffer Router 1IB with 15 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<15>("Shared_Buffer_Router_1IB_15SBs");
						break;
					case (20):
//						cout << "Binding Shared Buffer Router 1IB with 20 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<20>("Shared_Buffer_Router_1IB_20SBs");
						break;
					case (35):
//						cout << "Binding Shared Buffer Router 1IB with 35 Shared Buffers..." << endl;
						router = new SBR_1IB_Router<35>("Shared_Buffer_Router_1IB_35SBs");
						break;

					default:;
				}
*/
				break;
			case (ROUTER_LOWCOST_ONE_ENTRY):
//				router = new OneEntryBufferRouter("One_Entry_Buffer_Router");
				break;

			case (ROUTER_WORMHOLE_PIPELINE):
				router = new WormholePipeline("Wormhole_Pipeline");
				break;

			default:
				cout << "ERROR: Invalid Router Type!" << endl;
				exit (0);
				break;
		}


		router->clk(clk);
		router->reset(reset);

		router->valid_in[LOCAL](tx_valid_out);
		router->flit_in[LOCAL](tx_flit_out);
		for (int k=0; k<MAX_N_VCS; k++){
			router->in_vc_buffer_rd[LOCAL][k](tx_out_vc_buffer_rd[k]);
		}

		router->valid_out[LOCAL](rx_valid_in);
		router->flit_out[LOCAL](rx_flit_in);
//		router->out_full[LOCAL][0](rx_in_full);
		for (int k=0; k<MAX_N_VCS; k++){
			router->out_vc_buffer_rd[LOCAL][k](rx_in_vc_buffer_rd[k]);
		}

		for (int i=0; i<ROUTER_PORTS-1; i++){
			router->valid_in[i](valid_in[i]);
			router->flit_in[i](flit_in[i]);
			for (int k=0; k<MAX_N_VCS; k++){
				router->in_vc_buffer_rd[i][k](in_vc_buffer_rd[i][k]);
			}

			router->valid_out[i](valid_out[i]);
			router->flit_out[i](flit_out[i]);
			for (int k=0; k<MAX_N_VCS; k++){
				router->out_vc_buffer_rd[i][k](out_vc_buffer_rd[i][k]);
			}
		}

	}
};

#endif /* TILE_H_ */
