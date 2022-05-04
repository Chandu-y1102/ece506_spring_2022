/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	io_function.h
			Owner		:	Ed Gehringer
*************************************************************/

#ifndef IO_FUNCTION_H
#define IO_FUNCTION_H
//
int sharers(ulong addr);
void print_sim_stats(int cache_size, int cache_assoc, int blk_size,
					int num_processors,const char ** protocol_name,
					int protocol, char *fname, int dir_type);
void print_sim_results(	int np, Cache** processor_cache, int protocol,
						int dir_type);
//
#endif
