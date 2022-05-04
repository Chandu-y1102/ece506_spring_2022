/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	io_function.cc
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
using namespace std;
#include "cache.h"
#include "io_function.h"
//
//**	This function is used to print simulator configurations
//
void print_sim_stats(int cache_size, int cache_assoc, int blk_size,
					int num_processors, const char **protocol_names, 
					int protocol, char *fname, int dir_type){
	printf("===== SP19 506 DSM cache configuration =====\n");
	cout << left << setw(22) << "L1_SIZE:"<< cache_size << endl;
	cout << setw(22) << "L1_ASSOC:" << cache_assoc << endl;
	cout << setw(22) << "L1_BLOCKSIZE:" << blk_size << endl;
	cout << setw(22) << "NUMBER OF PROCESSORS:" << num_processors;
	cout << endl;
	cout << setw(22) << "COHERENCE PROTOCOL:" << protocol_names[dir_type]; 
	cout << endl;
	cout << setw(22) << "TRACE FILE:" << fname<< endl;
}
//
//** Print the final statistics for each processor cache
//
void print_sim_results(	int num_processors, 
						Cache** processor_cache,
						int protocol, int dir_type){
    for (int i=0; i<num_processors; i++)
		processor_cache[i]->print_stats(i, protocol, dir_type);
}
