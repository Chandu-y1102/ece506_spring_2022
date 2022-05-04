/************************************************************
                        Course          :       CSC506
                        Source          :       moesi.cc
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
#include "moesi.h"


/*
------------------------------------------------------
Don't modify the fucntions included in this section
------------------------------------------------------
*/
cache_line * MOESI::allocate_line_dir(ulong addr) {
	return NULL;
}
void MOESI::signalRd(ulong addr, int processor_number){
}
void MOESI::signalRdX(ulong addr, int processor_number){
}
void MOESI::signalUpgr(ulong addr, int processor_number){
}
void MOESI::Inv(ulong addr) {
}
void MOESI::Int(ulong addr) {
}
void MOESI::PrRdDir(ulong addr, int processor_number) {
}
void MOESI::PrWrDir(ulong addr, int processor_number) {
}
//-------------------------------------------------------------
//Section ends here. edit the functions in the section below 
//-------------------------------------------------------------

void MOESI::PrRd(ulong addr, int processor_number) 
{}


void MOESI::PrWr(ulong addr, int processor_number) 

{}


void MOESI::BusRd(ulong addr)

{}

void MOESI::BusRdX(ulong addr) 
{}


void MOESI::BusUpgr(ulong addr) 
{}


bool MOESI::writeback_needed(cache_state state) 
{
}

