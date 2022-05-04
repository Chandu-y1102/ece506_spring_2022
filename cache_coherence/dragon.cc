/********************
                        Course          :       CSC506
                        Source          :       dragon.cc
                        Instructor      :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------
        Â©️ Please do not replicate this code without consulting
                the owner & instructor! Thanks!
Code Integration : Tarun Govind Kesavamurthi
*********************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;
#include "main.h"
#include "dragon.h"


/*
------------------------------------------------------
Don't modify the fucntions included in this section
------------------------------------------------------
*/
cache_line * Dragon::allocate_line_dir(ulong addr) {
        return NULL;
}
void Dragon::signalRd(ulong addr, int processor_number){
}
void Dragon::signalRdX(ulong addr, int processor_number){
}
void Dragon::signalUpgr(ulong addr, int processor_number){
}
void Dragon::Inv(ulong addr) {
}
void Dragon::Int(ulong addr) {
}
void Dragon::PrRdDir(ulong addr, int processor_number) {
}
void Dragon::PrWrDir(ulong addr, int processor_number) {
}
//-------------------------------------------------------------
//Section ends here. edit the functions in the section below
//-------------------------------------------------------------

void Dragon::PrRd(ulong addr, int processor_number)
{
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
                        newline->set_state(Sc);
                }
                else {
                       memory_transactions++;
                        newline->set_state(E);
                }
        }
        else {
                state = line->get_state();
                if (state == Sm) {
                        update_LRU(line);
                }
                else if (state == Sc) {
                        update_LRU(line);
                }
                else if (state == M) {
                        update_LRU(line);
                }
                else if (state == E) {
                        update_LRU(line);
                }

        }

}

void Dragon::PrWr(ulong addr, int processor_number)
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
                    sendBusRd(addr, processor_number);
                    cache2cache++;
                   memory_transactions++;
                    newline->set_state(Sm);
                    bus_writes++;
                //    bus_upgrades++;
                  }
else {
                    sendBusRd(addr, processor_number);
                    memory_transactions++;
                    bus_reads++;
                    newline->set_state(M);  }}
else{
                    state = line->get_state();
                    int count = sharers(addr);
                    if (state == Sm ) {
                    if(count){
                              update_LRU(line);
                            //  bus_upgrades++;
                              bus_writes++;
                              sendBusUpgr(addr, processor_number); }
else {
                               line->set_state(M);
                              // bus_upgrades++;
                              bus_writes++;
                               sendBusUpgr(addr, processor_number);}}

else if(state == Sc){
         if(count){
                               line->set_state(Sm);
                               update_LRU(line);
                            //   bus_upgrades++
                            bus_writes++;
                               sendBusUpgr(addr, processor_number);}}
else if(state == M){
                              update_LRU(line);
}
else if(state == E){
                              update_LRU(line);
                              line->set_state(M);
}
}
}


void Dragon::BusRd(ulong addr)
{
    cache_state state;
    cache_line * line=find_line(addr);
    if (line!=NULL)
       {state = line->get_state();
		if (state == E){
			line->set_state(Sc);
      update_LRU(line);
			cache2cache++; }
		if (state == M){
    //  write_backs++;
        //  memory_transactions++;
			cache2cache++;
      line->set_state(Sm); }
    if(state == Sm){
  //    write_backs++;
    //      memory_transactions++;
      //    flushes++;
         }
	}
}


void Dragon::BusRdX(ulong addr)
{}


void Dragon::BusUpgr(ulong addr)
{

  cache_state state;
  cache_line *line = find_line(addr);
  if (line != NULL) {
      if(state==Sm){
          line->set_state(Sc);
        //   ++;//redo
  }
  else if(state==Sc){
    update_LRU(line);
  //  bus_upgrades++;
  //  memory_transactions++;
  }

  }
}

void Dragon::BusWr(ulong addr)
{
  cache_state state;
  cache_line * line=find_line(addr);
  if (line!=NULL)
     {state = line->get_state();
  if (state == E){
    line->set_state(Sc);
    update_LRU(line);
    cache2cache++; }
  if (state == M){
  //  write_backs++;
      //  memory_transactions++;
    cache2cache++;
    line->set_state(Sm); }
  if(state == Sm){
//    write_backs++;
  //      memory_transactions++;
        flushes++;  }
}
}
//}


bool Dragon::writeback_needed(cache_state state)
{

  if(state==Sm )
{//edit this function to return the correct boolean value
  write_backs--;
  memory_transactions--;
 cache2cache--;
return true;}
else{//memory_transactions--;
return false;}
}
