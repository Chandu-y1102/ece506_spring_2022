/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	fbv.cc
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/
#include "fbv.h"

void FBV::add_sharer_entry(int proc_num){
	bit[proc_num] = true;
}

void FBV::remove_sharer_entry(int proc_num){
	// YOUR CODE HERE
	// Reset the bit vector entry
	//for(int i=0;i< num_processors;i++){
        //if(i!=proc_num) 
         bit[proc_num] = false;
        //}

}

int FBV::is_cached(int num_proc){
    int count=0;
	// YOUR CODE HERE
	// Check bit vector for any set bit. 
	// If set, return 1, else send 0
	for(int i=0;i< num_proc;i++){
        if (bit[i] == true) count++;
	
        }
        return count;
}

void FBV::sendInt_to_sharer(ulong addr, int num_proc, int proc_num){
	// YOUR CODE HERE
	//
	// Invoke the sendInt function defined in main 
	// for all the processors except for proc_num
	// Make sure that you check the FBV to see if the 
	// bit is set
	for(int i=0;i<num_proc;i++){
          if(i!=proc_num && bit[i])
          sendInt(addr,i); 

        } 
	
}

void FBV::sendInv_to_sharer(ulong addr, int num_proc, int proc_num){
	// YOUR CODE HERE
	//
	// Invoke the sendInv function defined in main 
	// for all the processors except for proc_num
	// Make sure that you check the FBV to see if the 
	// bit is set
	for(int i=0;i<num_proc;i++){
          if(i!=proc_num && bit[i]){
          //if(i!=proc_num ){
            sendInv(addr,i);
            remove_sharer_entry(i); 
          }

        } 
}

