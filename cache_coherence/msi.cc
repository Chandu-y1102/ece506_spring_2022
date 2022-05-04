/************************************************************
                        Course          :       CSC456
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
#include "msi.h"

void MSI::PrRd(ulong addr, int processor_number) {
    cache_state state;
    current_cycle++;
    reads++;
    cache_line * line = find_line(addr);
       if (line == NULL) {
             read_misses++;
             cache_line *newline = allocate_line(addr);
             memory_transactions++;
                I2S++;
             newline->set_state(S);
             bus_reads++;
             sendBusRd(addr, processor_number);
       }
      else {
             state=line->get_state();
             if (state == I){
                        read_misses++;
                        cache_line *newline = allocate_line(addr);
                        memory_transactions++;
                        I2S++;
                        newline->set_state(S);
                        bus_reads++;
                        sendBusRd(addr, processor_number);
                }
             else if (state == M || state == S){
                update_LRU(line);
                }
             }
}


void MSI::PrWr(ulong addr, int processor_number) {
    cache_state state;
    current_cycle++;
    writes++;
    cache_line * line = find_line(addr);
    if (line == NULL || line->get_state() == I){
                write_misses++;
                cache_line *newline = allocate_line(addr);
                memory_transactions++;
                I2M++;
                newline->set_state(M);
                bus_readxs++;
                sendBusRdX(addr, processor_number);
     }
    else
        {
                state=line->get_state();
                if (state == M){
                update_LRU(line);
                }
                else if (state == S){
                S2M++;
                line->set_state(M);
                update_LRU(line);
                bus_readxs++;
                memory_transactions++;
                sendBusRdX(addr, processor_number);
                }
         }
}


void MSI::BusRd(ulong addr) {
    cache_state state;
    cache_line * line=find_line(addr);
    if (line!=NULL)
       {
                state = line->get_state();
                if (state == M)
                    {
                      flushes++;
                      write_backs++;
                      memory_transactions++;
                      interventions++;
                      line->set_state(S);
                      M2S++;
                    }
        }
}


void MSI::BusRdX(ulong addr) {
    cache_line * line=find_line(addr);
    if (line!=NULL)     {
                cache_state state;
                state=line->get_state();
                if (state == S )
                {
                invalidations++;
                line->set_state(I);
                }
                else if (state == M )
                {
                flushes++;
                invalidations++;
                write_backs++;
                memory_transactions++;
                line->set_state(I);
                }
        }
}


bool MSI::writeback_needed(cache_state state) {
    if (state == M)
    {
        return true;
    } else {
        return false;
    }
}

