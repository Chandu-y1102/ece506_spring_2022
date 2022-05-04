/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	ssci.cc
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/
#include "ssci.h"

void SSCI::add_sharer_entry(int proc_no){
	cache_list.push_back(proc_no);
}

void SSCI::remove_sharer_entry(int proc_num){
	// YOUR CODE HERE
	//
	// Remove the entry from the linked list
	cache_list.remove(proc_num);
}

int SSCI::is_cached(int proc_num){
	// YOUR CODE HERE
	//
	// Return 1 if the linked list is not empty
	// else return 0
	if(!(cache_list.empty())) return 1;
	else return 0; // Returning 0 to avoid compilation error
}

void SSCI::sendInv_to_sharer(ulong addr, int num_proc, int proc_num){
	// YOUR CODE HERE
	//
	// Erase the entry from the list except for the latest entry
	// The latest entry will be for the processor which is invoking
	// this function
	// Invoke the sendInv function defined in the main function
	for(int i=0; i < num_proc; i++) {
		if(i!=proc_num){
			sendInv(addr, i);
			remove_sharer_entry(i);
		}
	}
	
}

void SSCI::sendInt_to_sharer(ulong addr, int num_proc, int proc_num){
	// YOUR CODE HERE
	//
	// Invoke the sendInv function defined in the main function
	// for all the entries in the list except for proc_num. 
	for(int i=0; i < num_proc; i++) {
		if(i!=proc_num){
			sendInt(addr, i);
			//remove_sharer_entry(i);
		}
	}
}
