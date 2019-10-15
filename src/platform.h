/*
 * noc.h
 *
 * (C) copyright by the VCL laboratory, ECE Department, UC Davis
 *
 *  Please send en email to anhtrandavis@gmail.com to ask for your permission
 *  before using this code;
 *  and cite the paper "..." in your related publications.
 *
 *
 *  Created on: Apr 22, 2010
 *      Author: Anh Tran
 *
 *  A 2-D Mesh NoC of Tiles (PE + Router)
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "common_functions.h"

//#include <systemc.h>
//#include "synthetic/synthetic_tile.h"
//#include "asap/asap_tile.h"
#include "tile.h"

using namespace std;

/*
class BadConversion: public runtime_error {
  public:
	BadConversion(string const& s):runtime_error(s){ }
 };
*/

/*
inline string int_to_str(int x) {
	ostringstream o;
	if (!(o << x))
//		throw BadConversion("int_to_str(int)");
		throw runtime_error("int_to_str(int");
	return o.str();
}
*/

SC_MODULE (Platform){
	sc_in <bool> clk;
	sc_in <bool> reset;

	sc_signal <bool> W_valid_in[MAX_DIM][MAX_DIM];
	sc_signal <Flit> W_flit_in[MAX_DIM][MAX_DIM];
//	sc_signal <bool> W_in_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> W_in_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	sc_signal <bool> N_valid_in[MAX_DIM][MAX_DIM];
	sc_signal <Flit> N_flit_in[MAX_DIM][MAX_DIM];
//	sc_signal <bool> N_in_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> N_in_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	sc_signal <bool> W_valid_out[MAX_DIM][MAX_DIM];
	sc_signal <Flit> W_flit_out[MAX_DIM][MAX_DIM];
//	sc_signal <bool> W_out_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> W_out_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	sc_signal <bool> N_valid_out[MAX_DIM][MAX_DIM];
	sc_signal <Flit> N_flit_out[MAX_DIM][MAX_DIM];
//	sc_signal <bool> N_out_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> N_out_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	Tile *tile[MAX_DIM][MAX_DIM];

	SC_CTOR (Platform){
		for (int x=0; x<CommonParameter::dim_x; x++){
			for (int y=0; y<CommonParameter::dim_y; y++){
				string label = "Tile[";
				label = label + int_to_str(x) + "][" + int_to_str(y) + "]";

//				cout << "Binding " << label << "..." << endl;

				tile[x][y] = new Tile(label.data());

				tile[x][y]->proc->initialize(x, y);
				tile[x][y]->router->initialize(x, y);

				// initialize all constants
				tile[x][y]->initialize();
//				cout << "Done Tile Initialization" << endl;

				tile[x][y]->clk(clk);
				tile[x][y]->reset(reset);

/*
				switch (Parameter::platform_type){
					case (PLATFORM_SYNTHETIC):
						tile[x][y]->synth_pe->local_x = x;
						tile[x][y]->synth_pe->local_y = y;
						break;
					case (PLATFORM_ASAP):
						tile[x][y]->asap_proc->local_x = x;
						tile[x][y]->asap_proc->local_y = y;
						break;
					default:;
				}
*/
				// binding all interconnect signals
				for (int i=0; i<ROUTER_PORTS-1; i++){
					switch (i){
						case (WEST):
							tile[x][y]->valid_in[i](W_valid_in[x][y]);
							tile[x][y]->flit_in[i](W_flit_in[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](W_in_vc_buffer_rd[x][y][k]);
							}

							tile[x][y]->valid_out[i](W_valid_out[x][y]);
							tile[x][y]->flit_out[i](W_flit_out[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](W_out_vc_buffer_rd[x][y][k]);
							}
							break;
						case (EAST):
							tile[x][y]->valid_in[i](W_valid_out[x+1][y]);
							tile[x][y]->flit_in[i](W_flit_out[x+1][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](W_out_vc_buffer_rd[x+1][y][k]);
							}

							tile[x][y]->valid_out[i](W_valid_in[x+1][y]);
							tile[x][y]->flit_out[i](W_flit_in[x+1][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](W_in_vc_buffer_rd[x+1][y][k]);
							}
							break;
						case (NORTH):
							tile[x][y]->valid_in[i](N_valid_in[x][y]);
							tile[x][y]->flit_in[i](N_flit_in[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](N_in_vc_buffer_rd[x][y][k]);
							}

							tile[x][y]->valid_out[i](N_valid_out[x][y]);
							tile[x][y]->flit_out[i](N_flit_out[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](N_out_vc_buffer_rd[x][y][k]);
							}
							break;
						case (SOUTH):
							tile[x][y]->valid_in[i](N_valid_out[x][y+1]);
							tile[x][y]->flit_in[i](N_flit_out[x][y+1]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](N_out_vc_buffer_rd[x][y+1][k]);
							}

							tile[x][y]->valid_out[i](N_valid_in[x][y+1]);
							tile[x][y]->flit_out[i](N_flit_in[x][y+1]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](N_in_vc_buffer_rd[x][y+1][k]);
							}
							break;
						default:;
					}
				}

/*
				if (Parameter::platform_type == PLATFORM_ASAP){
					int tile_id = tile_loc_to_id(x, y);

//					cout << "HAHAHAHAHAHA...... tile_id = " << tile_id << endl;

					int task_id = AsAP_Parameter::app_map_reverse.find(tile_id)->second;

					cout << "task_id = " << task_id
							<< " is mapped at tile_id = " << tile_id
							<< " at location ( " << x << "," << y << ")" << endl;

					//------------ input constant values for AsAPProc
					tile[x][y]->asap_proc->is_used = AsAP_Parameter::app_map.find(task_id)->second.is_used;

					//------------ input constant values for AsAPCore
					tile[x][y]->asap_proc->is_input = AsAP_Parameter::app_map.find(task_id)->second.is_input;
					tile[x][y]->asap_proc->is_output = AsAP_Parameter::app_map.find(task_id)->second.is_output;

					tile[x][y]->asap_proc->core->n_srcs = AsAP_Parameter::app_map.find(task_id)->second.n_srcs;
					for (int i=0; i<tile[x][y]->asap_proc->core->n_srcs; i++){
						tile[x][y]->asap_proc->core->src[i] =  AsAP_Parameter::app_map.find(task_id)->second.input_info[i];
					}

					tile[x][y]->asap_proc->core->n_dsts = AsAP_Parameter::app_map.find(task_id)->second.n_dsts;
					for (int i=0; i<tile[x][y]->asap_proc->core->n_dsts; i++){
						tile[x][y]->asap_proc->core->dst[i] =  AsAP_Parameter::app_map.find(task_id)->second.output_info[i];
					}

					tile[x][y]->asap_proc->core->n_exe_cycles = AsAP_Parameter::app_map.find(task_id)->second.n_exe_cycles;
					tile[x][y]->asap_proc->core->initialize();	// compute n_delay_cycles

					//----------- input constant values for AsAPCoreToRouter
					tile[x][y]->asap_proc->core_to_router->task_id = task_id;
				}
*/
//				cout << "Binding " << label << "DONE!" << endl;
			}
		}
	}
};

#endif /* PLATFORM_H_ */
