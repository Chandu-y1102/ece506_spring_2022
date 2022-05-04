/************************************************************
                        Course          :       CSC506
                        Source          :       mosi.cc
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
#include "cache.h"
#include "mosi.h"

void MOSI::PrRd(ulong addr, int processor_number)
{
    cache_state state;
    current_cycle++;
    reads++;
    cache_line * line = find_line(addr);
    if (line == NULL) {

          read_misses++;
          cache_line *newline = allocate_line(addr);
          if(c2c_supplier(addr, processor_number) == 0){
                memory_transactions++;
          }
          else{
                cache2cache++;
          }
          I2S++;
          newline->set_state(S);
          bus_reads++;
          sendBusRd(addr, processor_number);

    }
    else {														//cache line exists
        state=line->get_state();
        if (state == I){

                  read_misses++;
                  cache_line *newline = allocate_line(addr);
                  if(c2c_supplier(addr, processor_number) == 0){
     	              memory_transactions++;
     	          }
     	          else{
     	            cache2cache++;
     	          }
                  I2S++;
                  newline->set_state(S);
                  bus_reads++;
                  sendBusRd(addr, processor_number);
               
        }
        else if (state == M || state == S ||  state == O){
           update_LRU(line);
        }
    }
}

void MOSI::PrWr(ulong addr, int processor_number)
{  cache_state state;
    current_cycle++;
    writes++;
    cache_line * line = find_line(addr);
    if (line == NULL || line->get_state() == I){
        write_misses++;
        cache_line *newline = allocate_line(addr);
        I2M++;
        newline->set_state(M);
        bus_readxs++;

        if(sharers_exclude(addr, processor_number) == 0) //no sharers
          memory_transactions++;
        else{
          if(c2c_supplier(addr, processor_number) == 0){
	    memory_transactions++;
	  }
	  else{
	    cache2cache++;
	  }
        }

        sendBusRdX(addr, processor_number);
    }
    else
    {
        state=line->get_state();
        if (state == M){
    	    update_LRU(line);
        }

        else if (state == S || state == O ){
    	    S2M++;
    	    line->set_state(M);
    	    update_LRU(line);
    	    bus_upgrades++;
    	    sendBusUpgr(addr, processor_number);
        }
     }
}

void MOSI::BusRd(ulong addr)
{  cache_state state;
    cache_line * line=find_line(addr);
    if (line!=NULL)
    {
      state = line->get_state();
      if (state == M)
      {
        flushes++;
        //write_backs++;
        //memory_transactions++;
        interventions++;
        line->set_state(O);
        M2O++;
      }

      else if (state == S)
      {
        line->set_state(S);
      }
      else if (state == O)
      {
        flushes++;
      }
    }
}

void MOSI::BusRdX(ulong addr)
{  cache_line * line=find_line(addr);
    if (line!=NULL) {
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
//-------------------
         write_backs++;
         memory_transactions++;
         line->set_state(I);
       }

       else if (state == O )
       {
         flushes++;
//-------------------
         write_backs++;
         memory_transactions++;
         invalidations++;
         //cache2cache++;
         line->set_state(I);
       }

    }
}

void MOSI::BusUpgr(ulong addr)
{ cache_line * line=find_line(addr);
   if (line!=NULL)     {
        cache_state state;
        state=line->get_state();
        if (state == S || state == O  )
        {
        invalidations++;
        line->set_state(I);
        }
   }}

bool MOSI::writeback_needed(cache_state state) {
  if (state == M || state == O)
  {
      return true;
  } else {
      return false;
  }
}
