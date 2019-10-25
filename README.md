NoCTweak: an On-Chip Network Simulator

Authors: Anh Tran (Andrew), and Bevan Baas

A networks-on-chip (NoC) simulator allows designers to early estimate performance (latency and throughput), energy efficiency (average/peak power, average energy per packet) and area of several networks on-chip configurations at different CMOS nodes. This tool is a cycle-accurate simulator and is open-source using SystemC, a C++ plugin, which is used to quickly model complex systems at a higher level but less details than RTL.

NoCTweak has been extended and integrated into McSim, a project developed by Dr. Abdoulaye Gamatié, Dr. Gilles Sassatelli, Dr. Manuel Selva et al. at LIRMM lab. McSim is a simulator for NoC-based many-core platforms, and is a part of the DreamCloud European project. DreamCloud allows dynamic resource allocation in many-core systems targeting high performance and energy efficient embedded applications.

DreamCloud and McSim link: https://github.com/DreamCloud-Project

+ Features
	- benchmarks: synthetic, real/embedded applications, multithread traces
	- topologies: mesh, torus, ring
	- router architectures: wormhole, virtual channel, circuit-switched, shared queues
	- routing algorithms: deterministic, adaptive, table-based
arbitrating policies: round-robin, iSLIP, winner takes all

+ Wormhole router with optional pipeline stages:
	- 1 stage: 
		. type 1:   BF+LRC/SA+ST+LT
	- 2 stages: 
		. type 2_1: BF | LRC/SA+ST+LT
		. type 2_2: BF+LRC/SA | ST+LT
		. type 2_3: BF+LRC/SA+ST | LT
	- 3 stages: 
		. type 3_1: BF | LRC/SA | ST+LT
		. type 3_2: BF | LRC/SA+ST | LT
		. type 3_3: BF+LRC/SA | ST | LT
	- 4 stages: 
		. type 4:   BF | LRC/SA | ST | LT
	- 5 (not necessary)
	
+ Simulation reports:
	- average packet delay and network throughput
	- average power of the router and its components
	- global clock tree + PLL according to the number of routers
	- average energy per packet	

Please cite the below paper in your related publications if you find NoCTweak useful to your work (PDF version of the paper is attached in the repository):

Anh Tran and Bevan Baas,
"NoCTweak: a Highly Parameterizable Simulator for Early Exploration of
Performance and Energy of Networks On-Chip,"
Technical Report, VCL Lab, ECE Department, UC Davis, July 2012.

