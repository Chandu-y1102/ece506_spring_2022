/************************************************************
                        Course          :       CSC/ECE506
                        Source          :       main.cc
                        Instructor      :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------
        Version |       Programmer              |       Change Description
------------------------------------------------------------
                1.0     |       Ahmad Samih &   |
                        |       Yan Solihin             |       Initial Version
                2.0     |       Ed Gehringer    |       Revised for Spring 2015
                        |                                       |       CSC/ECE506
------------------------------------------------------------
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
*************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <fstream>
using namespace std;
// Cache header file which contains cache super class and
// // related methods
#include "cache.h"
#include "main.h"
// Input-Output Header File to print simulator statistics
#include "io_function.h"
#define INVALIDATE 1

const int max_processors = 16;
int num_processors;
int blk_size, protocol;
Directory * directory;
Cache* processor_cache[max_processors];
//typedef enum {false, true} bool;

const char* protocol_names[ ] = {"MSI", "MOSI", "Firefly", "Dragon", "MOESI", "MSI_up", "MESI", "MI", "writethrough" /* "MESI with FBV", "MESI with SCI", "MOESI with FBV", "MOESI with SCI",*/};

//
////****************************************************************//
////*                        Main Function                         *//
////****************************************************************//
//

int main(int argc, char *argv[]){

	//char *project = (char *)malloc(20); 
	string project	= argv[1];
	//*** accept the simulator parameters from the command prompt ****//
	if (argv[2] == NULL){
        	print_input_format(project);
        	exit(0);
    	}

	unsigned long int instr_count = 200000000000;
	bool instn_limit = false;    //##check## new var added to limit instn
	//***** Move the parameters to the local variables *****//
	if (project == "smp"){
		int cache_size              = atoi(argv[2]);
    		int cache_assoc             = atoi(argv[3]);
    		blk_size                    = atoi(argv[4]);
    		num_processors              = atoi(argv[5]);
    		protocol                    = atoi(argv[6]);
        	char *fname                 = (char *)malloc(20);
    		fname                       = argv[7];	
		if (argv[8]) {
	            instn_limit = true;
		    instr_count             = atoi(argv[8]);
		}
		
		//*******Print out simulator configuration here*******//
        	print_sim_stats(cache_size, cache_assoc, blk_size, num_processors, protocol_names, protocol, fname);

		//*******Create an array of caches here*********//
		create_processor_cache(num_processors, cache_size, cache_assoc, blk_size, protocol);

		//********Process the trace file and simulate the cache*********//
		simulate_caches(fname, project, instr_count, instn_limit);

		//********Print out final statistics for all the processor caches **//
		print_sim_results(num_processors, processor_cache, protocol);

	}
	else if (project == "dsm"){
		int cache_size              = atoi(argv[2]);
    		int cache_assoc             = atoi(argv[3]);
    		blk_size                    = atoi(argv[4]);
    		num_processors              = atoi(argv[5]);
        	protocol                    = atoi(argv[6]);
    		int dir_type                = atoi(argv[7]);
        	char *fname                 = (char *)malloc(20);
    		fname                       = argv[8];
		if (argv[9]) {
			instr_count             = atoi(argv[9]);
		}

		int temp = 0;
        	if (protocol == 0 && dir_type == 0)
		{
                	temp = 6;
			protocol = 6;
		}
        	else if (protocol == 0 && dir_type == 1)
		{
        	        temp = 7;
			protocol = 6;
		}
        	else if (protocol == 1 && dir_type == 0)
		{
        	        temp = 8;
			protocol = 7;
		}
        	else if (protocol == 1 && dir_type == 1)
		{
        	        temp = 9;
			protocol = 7;
		}
		//*******Print out simulator configuration here*******//
		print_sim_stats_dir(cache_size, cache_assoc, blk_size, num_processors, protocol_names, protocol, fname, temp);

		//*******Create an array of caches here*********//
		ulong dir_size = (cache_size * num_processors) / (blk_size);
		//printf("protocol : %d\n temp : %d\n", protocol, temp);
		create_processor_cache_dir(num_processors, cache_size, cache_assoc, blk_size, protocol, dir_size, dir_type);

		//********Process the trace file and simulate the cache*********//
		simulate_caches(fname, project, instr_count, instn_limit);

		//********Print out final statistics for all the processor caches **//
		print_sim_results_dir(num_processors, processor_cache, protocol, dir_type);			
	}
}

//
////******************************************************************//
//*     Supporting methods for the main function
//******************************************************************//
//
void print_input_format(string project_num){

	if (project_num == "smp"){
		printf("input format: ");
	        printf("./simulate_cache <project_name> <cache_size> \n");
	        printf(" <assoc> <block_size> <num_processors> <protocol> <trace_file> \n");
	        printf(" *--------------------------------------*\n");
	        printf(" *\t Protocol Lookup Table \t\t*\n");
       	 	printf(" *--------------------------------------*\n");
        	printf(" * Protocol Number \t Protocol Name \t*\n");
        	printf(" *\t 0 \t\t MSI     \t*\n");
        	printf(" *\t 1 \t\t MOSI    \t*\n");
        	printf(" *\t 2 \t\t Firefly \t*\n");
		printf(" *\t 3 \t\t Dragon  \t*\n");
//		printf(" *\t 4 \t\t MOESI \t*\n");
//		printf(" *\t 5 \t\t MESI \t*\n");
        	printf(" *--------------------------------------*\n");
	}
	else if (project_num == "dsm"){
		printf("input format: ");
        	printf("./dsm <project_name> <cache_size> <assoc> <block_size>");
        	printf(" <num_processors> <dir_type> <trace_file> \n");
        	printf(" *----------------------------------------------*\n");
        	printf(" *\t Protocol Lookup Table \t\t\t*\n");
        	printf(" *----------------------------------------------*\n");
        	printf(" * Directory Type \t Protocol Name \t\t*\n");
        	printf(" *\t 0 \t\t MESI DSM with FBV \t*\n");
        	printf(" *\t 1 \t\t MESI DSM with SCI \t*\n");
        	printf(" *----------------------------------------------*\n");
	}

}

//
////**    This function is used to create cache for all the processors
////
//
void create_processor_cache(int num_processors, int size, int assoc, int blk_size, int protocol){
    	int i;
	// create cache for each processor based on
	// the protocol using abstract factory pattern
	for (i=0;i<num_processors;i++)
                processor_cache[i] = cache_factory::create_cache(protocol, size, assoc, blk_size, num_processors, i);

}

//
//////**    This function is used to create cache for all the processors
//////
////
void create_processor_cache_dir(int num_processors, int size, int assoc, int blk_size, int protocol, ulong directory_size, int dir_type){
	int i;
        directory = new Directory(directory_size, dir_type);
	// create cache for each processor based on the
	// protocol using abstract factory pattern
	for (i=0;i<num_processors;i++)
        {
                processor_cache[i] = cache_factory::create_cache(protocol, size, assoc, blk_size, num_processors, i);
                processor_cache[i]->set_directory(directory);
        }
}

//
////      This function performs the following
////      [1] Opens the file to be read and reports an error if open fails
////      [2]     Reads the trace file,line by line, each
////  (processor#, operation, address) propagate each request down
////      through memory hierarchy by calling cachesArray[processor#]->Access(...)        
////
void simulate_caches(char *fname, string project, int instr_count, bool instn_limit){   //##check## function params changed
	ifstream fin;
    	FILE * pFile;
        int proc_no, addr;
    	char op;
	int line_count = 0;
	// Print error if the file cannot be found or opened
	pFile = fopen (fname,"r");
	if (pFile == 0)     {
                printf("Trace file problem\n");
                exit(0);
   	 }
	// Read each entry from the trace file and perform
	// the relevant operation (read/write)
	// based on the input
	while (fscanf(pFile, "%d %c %x", &proc_no, &op, &addr) != EOF) {
		line_count++;
		if (op == 'w')
		{
			if (project == "smp")
				processor_cache[proc_no]->PrWr((ulong) addr, proc_no);
			else if (project == "dsm")
			{
				//printf("in write \n");
				processor_cache[proc_no]->PrWrDir((ulong) addr, proc_no);
			}
		}
		else
		{
			if (project == "smp")
				processor_cache[proc_no]->PrRd((ulong) addr, proc_no);
			else if (project == "dsm")
			{
				processor_cache[proc_no]->PrRdDir((ulong) addr, proc_no);
				//printf("in read \n");
			}
		}
		if ((instn_limit == true)&&(line_count == instr_count))   //##check## if condition changed
			break;
	}
	fclose(pFile);

}

//
////      Return the number of sharers of the block
////
int sharers(ulong addr) {
        int count = 0;
        for (int i = 0; i < num_processors; i++){
                if (processor_cache[i]->find_line(addr) != NULL)
                        count++;
        }
        return count;
}
//
////      Return the number of sharers of the block other than the processor calling this fucntion
////
int sharers_exclude(ulong addr, int proc_no) {
        int count = 0;
        for (int i = 0; i < num_processors; i++){
                if (i!=proc_no) {
                  if (processor_cache[i]->find_line(addr) != NULL) {
                        if (processor_cache[i]->find_line(addr)->get_state() != I) {
                        count++;
                        }
                  }
                }
        }
        return count;
}
//This function should tell if the cache block can be supplied by another cache; Note that in case of MSI/MOSI/MOESI, if all the processors are in S state or I state, the cache block has to come from main Memory. Only, M or O act as suppliers of cache blocks.
int c2c_supplier(ulong addr, int proc_no) {
        int count = 0;
        for (int i = 0; i < num_processors; i++){
                if (i!=proc_no) 
                 {
                   if (processor_cache[i]->find_line(addr) != NULL) 
                    {
			if ((processor_cache[i]->find_line(addr)->get_state() == M)||(processor_cache[i]->find_line(addr)->get_state() == O)||(processor_cache[i]->find_line(addr)->get_state() == E)) 
			{
			count++;
                        }
			//Implement the function such that if you have a processor that can supply the cache block to another processor, this function returns a non-zero value otherwise a 0.
			//Usethis function inside your <protocol.cc> to determine if you should increment a cache2cache transfer or memory transaction
                  }
              }
        }
        return count;
}

void sendBusRd(ulong addr, int proc_no){
                for (int i=0; i<num_processors; i++){
                        if (i!=proc_no)
                                processor_cache[i]->BusRd((ulong) addr);
                }
}

void sendBusWr(ulong addr, int proc_no){
                for (int i=0; i<num_processors; i++){
                        if (i!=proc_no)
                                processor_cache[i]->BusWr((ulong) addr);
                }
}

void sendBusRdX(ulong addr, int proc_no){
                for (int i=0; i<num_processors; i++){
                        if (i!=proc_no)
                                processor_cache[i]->BusRdX((ulong) addr);
                }
}

void sendBusUpgr(ulong addr, int proc_no){
                for (int i=0; i<num_processors; i++){
                        if (i!=proc_no)
                                processor_cache[i]->BusUpgr((ulong) addr);
                }
}

void sendInv(ulong addr, int proc_num){
        processor_cache[proc_num]->Inv(addr);
}

void sendInt(ulong addr, int proc_num) {
        processor_cache[proc_num]->Int(addr);
}

