/************************************************************
			Course		: 	CSC456
			Source 		:	main.cc
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <fstream>
using namespace std;
#include "cache.h"			
#include "main.h"			
#include "io_function.h"

const int max_processors = 16;
int num_processors;
int blk_size, protocol;
Directory * directory;
Cache* processor_cache[max_processors];

// Look up table to print the protocol names 
const char* protocol_names[ ] = { "MESI with FBV", "MESI with SCI" };

//*************************************************************
//*				Main Function								  *//
//*************************************************************//

int main(int argc, char *argv[]){

	//***** accept the simulator parameters from the command prompt
    if (argv[1] == NULL){
	print_input_format();
	exit(0);
    }
	int instr_count = 20000000;
	//***** Move the parameters to the local variables
    int cache_size 		= atoi(argv[1]);
    int cache_assoc		= atoi(argv[2]);
    blk_size	   		= atoi(argv[3]);
    num_processors 		= atoi(argv[4]);
	protocol			= 0;
    int dir_type			= atoi(argv[5]);	
	char *fname 		= (char *)malloc(20);
    fname 				= argv[6];
	if (argv[7]) {
			instr_count             = atoi(argv[7]);
		}
	//
	//*******Print out simulator configuration here*******//
	print_sim_stats(cache_size, cache_assoc, 
					blk_size, num_processors,
					protocol_names, protocol, fname, dir_type);

	//*******Create an array of caches here*********//
	ulong dir_size = (cache_size * num_processors) / blk_size;
	create_processor_cache( num_processors, cache_size,
							cache_assoc, blk_size, protocol,
							dir_size, dir_type);

	//********Process the trace file and simulate the cache*********//
	simulate_caches(fname, instr_count);

	//********Print out final statistics for all the processor caches **//
	print_sim_results(num_processors, processor_cache, protocol, dir_type);
}
//
//***********************************************************************//
//* 							Supporting methods for the main function
//***********************************************************************//
//
void print_input_format(){
	printf("input format: ");
	printf("./dsm <cache_size> <assoc> <block_size>");
	printf(" <num_processors> <dir_type> <trace_file> \n");
	printf(" *----------------------------------------------*\n");
	printf(" *\t Protocol Lookup Table \t\t\t*\n");
	printf(" *----------------------------------------------*\n");
	printf(" * Directory Type \t Protocol Name \t\t*\n");
	printf(" *\t 0 \t\t MESI DSM with FBV \t*\n");
	printf(" *\t 1 \t\t MESI DSM with SCI \t*\n");
	printf(" *----------------------------------------------*\n");
}
//
//**	This function is used to create cache for all the processors 
//
void create_processor_cache(int num_processors, int size, int assoc, 
							int blk_size, int protocol, ulong directory_size, int dir_type){
	int i;
	directory = new Directory(directory_size, dir_type);

	// create cache for each processor based on the 
	// protocol using abstract factory pattern
	for (i=0;i<num_processors;i++)
	{
		processor_cache[i] = cache_factory::create_cache(protocol, size, assoc,
													blk_size, num_processors,
													directory_size, i);
		processor_cache[i]->set_directory(directory);
	}
}
//
//**	This function performs the following			 	
//**	[1] Opens the file to be read and reports an 
//** 	error if open fails
//**  	[2]	Reads the trace file,line by line, each 
//** 	(processor#, operation, address) propagate each request down 
//**	through memory hierarchy by calling 
//**	cachesArray[processor#]->Access(...)
//
void simulate_caches(char *fname, int instr_count){
    ifstream fin;
    FILE * pFile;
	int proc_no;
    char op;
    int addr;
	int line_count = 0;
	// Print error if the file cannot be found or opened
    pFile = fopen (fname,"r");
    if (pFile == 0)	{	
		printf("Trace file problem\n");
		exit(0);
    }
   
	// Read each entry from the trace file and perform 
	// the relevant operation (read/write) based on the input
    while (fscanf(pFile, "%d %c %x", &proc_no, &op, &addr) != EOF) {
		line_count++;
		if (op == 'w')
			processor_cache[proc_no]->PrWr((ulong) addr, proc_no);
		else 
			processor_cache[proc_no]->PrRd((ulong) addr, proc_no);	
		if (line_count == instr_count)
			break; 	
    }
    fclose(pFile);
}
//
//	Return the number of sharers of the block
//
int sharers(ulong addr) {
	int count = 0;
	for (int i = 0; i < num_processors; i++){
		if (processor_cache[i]->find_line(addr) != NULL) 
			count++;
	}
	return count;
} 
//
void sendInv(ulong addr, int proc_num){
	processor_cache[proc_num]->Inv(addr);
}
//
void sendInt(ulong addr, int proc_num) {
	processor_cache[proc_num]->Int(addr);
}
