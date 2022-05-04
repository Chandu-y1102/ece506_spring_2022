/************************************************************
                        Course          :       CSC506
                        Source          :       mesi.cc
                        Instructor      :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
*************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;
#include "main.h"
#include "mesi.h"
#include "cache.h"

void MESI::PrRd(ulong addr, int processor_number) 
{}

void MESI::PrRdDir(ulong addr, int processor_number) {

}

void MESI::PrWrDir(ulong addr, int processor_number) {
}


void MESI::PrWr(ulong addr, int processor_number) 
{}


void MESI::BusRd(ulong addr) 
{}

void MESI::BusRdX(ulong addr) 
{}


void MESI::BusUpgr(ulong addr) 
{}



bool MESI::writeback_needed(cache_state state) 
{}

void MESI::signalRd(ulong addr, int processor_number){

	
}


void MESI::signalRdX(ulong addr, int processor_number){
	
}

void MESI::signalUpgr(ulong addr, int processor_number){

}

void MESI::Int(ulong addr) {
	
}


void MESI::Inv(ulong addr) {
	
}


cache_line * MESI::allocate_line_dir(ulong addr) {
}
