/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	mesi.cc
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;
#include "cache.h"

#include "main.h"
#include "mesi.h"
//
void MESI::PrRd(ulong addr, int processor_number) {
	// YOUR CODE HERE
	// The below comments are for reference and might not be
	// sufficient to match the debug runs.
	//
	// Update the Per-cache global counter to maintain LRU
	// order among cache ways, updated on every cache access
	// Increment the global read counter.
	// Check whether the line is cached in the processor cache.
	// If not cached, allocate a fresh line and update the state. (M,E,S,I)
	// Check whether the directory entry is updated. If not updated,
	// create a fresh entry in the directory, update the sharer vector or list.
	// Update the directory state (U, EM, S_).
	// Increment the directory operation counter like signalrds,
	// response_replies etc... Invoke the relevant directory
	// signal functions like signalRd or signalRdX etc...
	// If the line is cached in the processor cache, do not forget
	// to update the LRU
	// Do not forget to update miss/hit counter
    cache_state state;
    current_cycle++;
    reads++;
    cache_line * line = find_line(addr);
    dir_entry* dir_line;
    if (line == NULL) {
          read_misses++;
          cache_line *newline = allocate_line(addr);

          //signal_rds++;
          signalRd(addr,processor_number);
          //cout << addr << tag_field(addr) << "is not present in cache" << endl;
          dir_line = directory->find_dir_line(tag_field(addr));
          if(dir_line-> get_state()==EM){
			  I2E++;
			  newline->set_state(E);
	  }
          else if(dir_line-> get_state()==S_){
			  I2S++;
			  newline->set_state(S);
	  }
    }
    else {
    	state = line->get_state();
    	if(state==I) {
    		read_misses++;
    		cache_line *newline = allocate_line(addr);
    		//signal_rds++;
    		signalRd(addr,processor_number);
                dir_line = directory->find_dir_line(tag_field(addr));
    		if(dir_line-> get_state()==EM){
    			I2E++;
    			newline->set_state(E);
    		}
    		else if(dir_line-> get_state()==S_){
    			I2S++;
    			newline->set_state(S);
    		}

    	}
    	else if (state == M || state == S || state == E){
          update_LRU(line);
       }
    }

}
//
void MESI::PrWr(ulong addr, int processor_number) {
	// YOUR CODE HERE
	// Refer comments for PrRd
    cache_state state;
    current_cycle++;
    writes++;
    cache_line * line = find_line(addr);

    if (line == NULL) {
        write_misses++;
        cache_line *newline = allocate_line(addr);
        I2M++;
        //signal_rdxs++;
        signalRdX(addr, processor_number);
        newline->set_state(M);
    }
    else {
	state = line->get_state();
	if(state==I) {
           write_misses++;
           cache_line *newline = allocate_line(addr);
           I2M++;
           //signal_rdxs++;
           signalRdX(addr, processor_number);
           newline->set_state(M);
        }
        else if (state == M) {
	   update_LRU(line);
        }
    	else if (state == E){
    	   E2M++;
    	   line->set_state(M);
    	   update_LRU(line);
        }
        else if (state == S){
    	   S2M++;
    	   signal_upgrs++;
    	   signalUpgr(addr, processor_number);
    	   line->set_state(M);
    	   update_LRU(line);
        }
    }

}
//
cache_line * MESI::allocate_line(ulong addr) {
    ulong tag;
    cache_state state;

    cache_line *victim = find_line_to_replace(addr);
    assert(victim != 0);
    state = victim->get_state();
    if (state == M){
        write_back(addr);}

	ulong victim_tag = victim->get_tag();
	dir_entry* dir_line = directory->find_dir_line(victim_tag);
	if(dir_line!=NULL)	{
		dir_line->remove_sharer_entry(cache_num);
		int present = 0;
		present = dir_line->is_cached(num_processors);
		if(present==0) dir_line->state = U;
                //else if (present ==1)  dir_line->state = EM;
                //else dir_line->state = S_;

	}

    tag = tag_field(addr);
    victim->set_tag(tag);
    victim->set_state(I);
    return victim;
}
//
void MESI::signalRd(ulong addr, int processor_number){
      // YOUR CODE HERE
	// The below comments are for reference and might not be
	// sufficient to match the debug runs.
	//
	// Check the directory state and update the cache2cache counter
	// Update the directory state
	// Update the vector/list
	// Send Intervention or Invalidation
      dir_entry* dir_line;
      dir_state d_state;
      //cache_line* line = find_line(addr);
      //ulong line_tag = line->get_tag();
      //dir_line = directory->find_dir_line(line_tag);
      dir_line = directory->find_dir_line(tag_field(addr));
      if(dir_line==NULL){
          //dir_line=directory->find_empty_line(line_tag);
          dir_line=directory->find_empty_line(tag_field(addr));
      }
      if( dir_line==NULL){cout << "No empty line is present RD "<< endl; exit(1);     }
      d_state = dir_line-> get_state();

      if(d_state==U) {
           dir_line-> set_dir_state(EM);
           memory_transactions++;
           response_replyds++;
           dir_line->add_sharer_entry(processor_number);
      }
      else if(d_state== EM) {
           signal_rds++;
           dir_line->set_dir_state(S_);
           cache2cache++;
           flushes++;
           dir_line->add_sharer_entry(processor_number);
           dir_line->sendInt_to_sharer(addr,num_processors,processor_number);
      }
      else if(d_state== S_) {
           signal_rds++;
           memory_transactions++;
           response_replyds++;
           dir_line->add_sharer_entry(processor_number);
      }



}

void MESI::signalRdX(ulong addr, int processor_number){
	// YOUR CODE HERE
	// Refer to signalRd description in the handout
         dir_entry* dir_line;
         dir_state d_state;
	 //cache_line* line = find_line(addr);
	 //ulong line_tag = line->get_tag();
         //dir_line = directory->find_dir_line(line_tag);
         dir_line = directory->find_dir_line(tag_field(addr));
         if(dir_line==NULL){
             //dir_line=directory->find_empty_line(line_tag);
             dir_line=directory->find_empty_line(tag_field(addr));
         }


      if( dir_line==NULL){cout << "No empty line is present RDX "<< endl; exit(1);     }
         d_state = dir_line-> get_state();
         if(d_state==U) {
              dir_line-> set_dir_state(EM);
              memory_transactions++;
              response_replyds++;
              dir_line->add_sharer_entry(processor_number);
         }
         else if(d_state== EM) {
              signal_rdxs++;
              cache2cache++;
              flushes++;
              response_replies++;
              dir_line->add_sharer_entry(processor_number);
              dir_line->sendInv_to_sharer(addr,num_processors,processor_number);
              //dir_line->remove_sharer_entry(processor_number);
         }
         else if(d_state== S_) {
              signal_rdxs++;
              memory_transactions++;
              response_replyds++;
              dir_line-> set_dir_state(EM);
              dir_line->add_sharer_entry(processor_number);
              dir_line->sendInv_to_sharer(addr,num_processors,processor_number);
              //dir_line->remove_sharer_entry(processor_number);
         }

}

void MESI::signalUpgr(ulong addr, int processor_number){
	// YOUR CODE HERE
	// Refer to signalUpgr description in the handout
	 dir_entry* dir_line;
         dir_state d_state;
         dir_line = directory->find_dir_line(tag_field(addr));

         if(dir_line==NULL){
             //dir_line=directory->find_empty_line(line_tag);
             dir_line=directory->find_empty_line(tag_field(addr));
         }



         if( dir_line==NULL){cout << "No empty line is present upgr"<< endl; exit(1);     }
         d_state = dir_line-> get_state();

         if(d_state== S_) {
              response_replies++;
              dir_line-> set_dir_state(EM);
              dir_line->sendInv_to_sharer(addr,num_processors,processor_number);
              dir_line->add_sharer_entry(processor_number);
         }
         if(d_state!= S_) {
              dir_line-> set_dir_state(EM);
              dir_line->add_sharer_entry(processor_number);
         }


}

void MESI::Int(ulong addr) {
	// YOUR CODE HERE
	// The below comments are for reference and might not be
	// sufficient to match the debug runs.
	//
	// Update the relevant counter, if the cache copy is dirty,
	// same needs to be written back to main memory. This is
	// achieved by simply updating the writeback counter
	cache_line * line = find_line(addr);
	cache_state state;
	if(line != NULL) {
	   state = line->get_state();
           if(state==M || state==E){
           //if(state==M){
             line->set_state(S);
             write_backs++;
             interventions++;
           }
	}

}

void MESI::Inv(ulong addr) {
	// YOUR CODE HERE
	// Refer Inv description in the handout
	//dir_entry* dir_line;
	cache_line * line = find_line(addr);
    //dir_line = directory->find_dir_line(tag_field(addr));
	cache_state state;
	if(line != NULL) {
		state = line->get_state();
		if(state==M || state==E || state==S){
			if(line->get_state()!=S) write_backs++;
			//if(line->get_state()==M) write_backs++;
			line->set_state(I);
			invalidations++;
			inv_acks++;
        }
	}

}
