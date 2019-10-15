NoCTweak: an On-Chip Network Simulator

Written on: 2012/03/25
By: Anh Tran, UC Davis

Please cite the below paper in your related publications if you find NoCTweak useful to your work (PDF version of the paper is also in the repository):

Anh Tran and Bevan Baas,
"NoCTweak: a Highly Parameterizable Simulator for Early Exploration of
Performance and Energy of Networks On-Chip," 
Technical Report, VCL Lab, ECE Department, UC Davis, July 2012.

Wormhole router with optional pipeline stages:
	- 1 stage: 
		. type 1:	BF+LRC/SA+ST+LT
	- 2 stages: 
		. type 2_1: BF | LRC/SA+ST+LT
		. type 2_2: BF+LRC/SA | ST+LT
		. type 2_3: BF+LRC/SA+ST | LT
	- 3 stages: 
		. type 3_1: BF | LRC/SA | ST+LT
		. type 3_2: BF | LRC/SA+ST | LT
		. type 3_3: BF+LRC/SA | ST | LT
	- 4 stages: 
		. type 4:	BF | LRC/SA | ST | LT
	- 5 (not necessary)
	
+ Features;
	- report average packet delay and network throughput
	- report average power of the router and its components
	- report global clock tree + PLL according to the number of routers
	- report average energy per packet	

+ Updated:
	- 2012/06/26:
		. input voltage 
		. max frequency (min clock period) changes corresponding to the pipeline type and voltage 

