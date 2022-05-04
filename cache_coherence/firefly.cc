/************************************************************
                        Course          :       CSC506
                        Source          :       msi.cc
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
#include "firefly.h"


/*
------------------------------------------------------
Don't modify the fucntions included in this section
------------------------------------------------------
*/
cache_line * Firefly::allocate_line_dir(ulong addr) {
	return NULL;
}
void Firefly::signalRd(ulong addr, int processor_number){
}
void Firefly::signalRdX(ulong addr, int processor_number){
}
void Firefly::signalUpgr(ulong addr, int processor_number){
}
void Firefly::Inv(ulong addr) {
}
void Firefly::Int(ulong addr) {
}
void Firefly::PrRdDir(ulong addr, int processor_number) {
}
void Firefly::PrWrDir(ulong addr, int processor_number) {
}
//-------------------------------------------------------------
//Section ends here. edit the functions in the section below 
//-------------------------------------------------------------

void Firefly::PrRd(ulong addr, int processor_number) 
{
        //Processor Read
        cache_state state;
        reads++;
        current_cycle++;
        cache_line *line = find_line(addr);
        if (line == NULL) {
                read_misses++;
                cache_line *newline = allocate_line(addr);
                bus_reads++;
                sendBusRd(addr, processor_number);
                int count = sharers(addr);
                if (count) {
                        cache2cache++;
                        newline->set_state(S);
                }
                else {
                        memory_transactions++;
                        newline->set_state(V);
                }
        }
        else {
                // V, D, & S all have up to date cache so no bus/mem read happens
                state = line->get_state();
                if (state == V) {
                        update_LRU(line);
                }
                else if (state == D) {
                        update_LRU(line);
                }
                else if (state == S) {
                        update_LRU(line);
                }
        }
}

void Firefly::PrWr(ulong addr, int processor_number) 
{
        cache_state state;
        current_cycle++;
        writes++;
        cache_line *line = find_line(addr);
        if (line == NULL) {
                write_misses++;
                cache_line *newline = allocate_line(addr);
                int count = sharers(addr);
                if (count) {
                        cache2cache++;
                        newline->set_state(S);
                        bus_writes++;
                        sendBusWr(addr, processor_number);
                        memory_transactions++;//write to memory
                }
                else {
                        memory_transactions++; //Get from memory
                        bus_reads++;
                        sendBusRd(addr, processor_number);
                        newline->set_state(D);
                }
        }
        else {
                state = line->get_state();
                if (state == V) {
                        update_LRU(line);
                        line->set_state(D);
                }
                else if (state == D) {
                        update_LRU(line);
                }
                else if (state == S) {
                        update_LRU(line);
                        bus_writes++;
						int shared_line = sharers(addr) > 1;
						if (shared_line) { 
							line->set_state(S);
						}
						else{
							line->set_state(V);
						}
                        sendBusWr(addr, processor_number);
                        memory_transactions++;
                }
                else {
                        //Else: You're not in the three valid states
                        cout << "PrWr: Invalid State " << state << endl;
                }
        }
}


void Firefly::BusRd(ulong addr)
{
        cache_state state;
        cache_line *line = find_line(addr);
        if (line != NULL) {
                state = line->get_state();
                if (state == V) {
                        line->set_state(S);
                }
                else if (state == D) {
                        write_backs++;
                        memory_transactions++;
                        line->set_state(S);
                }
                else if (state == S) {
                        //update_LRU(line);
                }
                else {
                        //Else: You're not in the three valid states
                        cout << "BusRd: Invalid State " << state << endl;
                }
        }
}


void Firefly::BusRdX(ulong addr) 
{
   /*cache_line * line=find_line(addr);
   if (line!=NULL) { 
	cache_state state; 
        state = line->get_state();
	if (state == M)
          {
           invalidations++;
	   flushes++;
	   line->set_state(I);
	  }
        else if (state == O)
          {
	   invalidations++;
           write_backs++;
	   memory_transactions++;
	   flushes++;
	   line->set_state(I);
          }
         else if(state == E)
          {
            line->set_state(I);
            invalidations++;
          }
        else if(state == S)
          {
            line->set_state(I);
            invalidations++;
          }
       
    }*/ 
}


void Firefly::BusUpgr(ulong addr) 
{
    /* cache_line *line = find_line(addr);
    if(line!=NULL)
    {
      cache_state state;
      state = line->get_state();
      if(state == O || state == S)
       {
        line->set_state(I);
        invalidations++;
       }
     }*/
}

void Firefly::BusWr(ulong addr) 
{
       cache_state state;
        cache_line *line = find_line(addr);
        if (line != NULL) {
                state = line->get_state();
                if (state == V) {
                        line->set_state(S);
                }
                else if (state == D) {
                        write_backs++;
                        memory_transactions++;
                        line->set_state(S);
                }
                else if (state == S) {
                        //update_LRU(line);
                }
                else {
                        //Else: You're not in the three valid states
                        cout << "BusWr: Invalid State " << state << endl;
                }
        }
}

bool Firefly::writeback_needed(cache_state state) 
{
         if(state == D)
	//edit this function to return the correct boolean value
	   return true;
          else
           return false;
}

