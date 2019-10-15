/*
 * embedded_parameters.h
 *
 *  Created on: Aug 17, 2011
 *      Author: anhttran
 */

#ifndef EMBEDDED_PARAMETERS_H_
#define EMBEDDED_PARAMETERS_H_

//#include <tr1/unordered_map>
#include <map>

#include "../../common_parameters.h"

#define MAPPING_RANDOM 1
#define MAPPING_NMAP 2

#define MAX_N_DESTS	8 // maximum number of destinations of each task


struct DestInfo{	// destination info
//	int dst_task_id;
	int dst_tile_id;
	double bandwidth;
};

// destination information for each tile
// a tile may have multiple destinations
struct TileInfo{
//	TileLocation tile_loc;

	int n_dsts;
	DestInfo dst_info[MAX_N_DESTS];
};

// Hashtable <K=tile_id, V=TileInfo>
//typedef std::tr1::unordered_map<int, TileInfo> EmbeddedAppHashTable;
typedef map<int, TileInfo> EmbeddedAppHashTable;


struct EmbeddedParameters{
	static char *app_folder;
	static char *app_filename;

	static int mapping_algorithm;

	static double max_req_bandwidth;		// max required bandwidth on all links
	static EmbeddedAppHashTable app_info;	// overall application info after task mapping
};

#endif /* EMBEDDED_PARAMETERS_H_ */
