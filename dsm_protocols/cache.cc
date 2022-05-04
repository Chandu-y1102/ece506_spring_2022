/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	cache.cc
			Owner		:	Ed Gehringer
*************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <iomanip>
using namespace std;

#include "main.h"
#include "cache.h"
#include "mesi.h"

Cache::Cache(int s, int a, int b, int p, int pn){
	ulong i, j;
   // counters
   reads = read_misses = writes = 0;
   write_misses = write_backs = current_cycle = 0;
   memory_transactions = cache2cache = 0;
   signal_rds = signal_rdxs = signal_upgrs = 0;
   response_replies = response_replyds = flushes = 0;
   inv_acks = acks = invalidations = interventions = 0;

   num_processor	= (ulong) (p);
   size	      		= (ulong)(s);
   line_size   		= (ulong)(b);
   assoc      		= (ulong)(a);
   sets	      		= (ulong)((s/b)/a);
   num_lines   		= (ulong)(s/b);
   log2_sets   		= (ulong)(log2(sets));
   log2_blk    		= (ulong)(log2(b));
   cache_num		= pn;
   num_processors	= p;

   tag_mask =0;
   for (i=0;i<log2_sets;i++)  {
	tag_mask <<= 1;
	tag_mask |= 1;
   }

   /** Create a 2-dimensional cache, sized as cache[sets][assoc] **/
   cache = new cache_line*[sets];
   for (i=0; i<sets; i++)   {
      cache[i] = new cache_line[assoc];
      for (j=0; j<assoc; j++)
		cache[i][j].set_state(I);
   }
}

/* Look up line */
cache_line * Cache::find_line(ulong addr){
   ulong i, j, tag, pos;

   pos = assoc;
   tag = tag_field(addr);
   i   = index_field(addr);

   for (j=0; j<assoc; j++){
    if (cache[i][j].valid())
	if (cache[i][j].get_tag() == tag) {
	    pos = j; break;
	}}
    if (pos == assoc)
	return NULL;
    else
	return &(cache[i][pos]);
}

/* Upgrade LRU line to be MRU line */
void Cache::update_LRU(cache_line *line)
{
  line->set_seq(current_cycle);
}

/* Return an invalid line, if any; otherwise return the LRU line */
cache_line * Cache::get_line_to_evict(ulong addr)
{
   ulong i, j, victim, min;

    victim = assoc;
    min	  = current_cycle;
    i	  = index_field(addr);

    for (j=0; j<assoc; j++)
    {
       if (!cache[i][j].valid()) return &(cache[i][j]);
    }
    for (j=0; j<assoc; j++)   {
	if (cache[i][j].get_seq() <= min) {
	    victim = j; min = cache[i][j].get_seq();}
    }
    assert(victim != assoc);

    return &(cache[i][victim]);
}

/* Find a victim, and move it to the MRU position */
cache_line *Cache::find_line_to_replace(ulong addr)
{
   cache_line * victim = get_line_to_evict(addr);
   update_LRU(victim);
   return (victim);
}
//
// 	Factory Pattern to create the cache, based on the protocol
//	number input through the command prompt
Cache* cache_factory::create_cache(int protocol_no, int size,
									int assoc, int block_size, int num_processors,
									ulong directory_size, int processor_num){
	 switch ((protocol_name) protocol_no) {
			case MESI_dsm:
				return new MESI(	size, assoc, block_size,
											num_processors, processor_num);
			default:
				return NULL;
	}
}
//
void Cache::print_stats(int processor_number, int protocol, int dir_type){
	int count = 1;

	cout << "============ Simulation results (Cache ";
	cout << processor_number << ") ============ " << endl;
	cout << left << setw(2) << count++ << setw(40) <<". number of reads: ";
	cout << (int) get_reads();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of read misses: ";
	cout << (int) get_read_misses();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of writes: ";
	cout << (int) get_writes();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of write misses:";
	cout << (int) get_write_misses();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". total miss rate: ";
	printf("%.6lf", ((double) get_read_misses() +
					(double) get_write_misses()) / ((double) get_reads()
					+ (double) get_writes()));
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of writebacks: ";
	cout << (int) get_write_backs();
	cout << endl;
	/*
	cout << setw(2) << count++ << setw(40) <<". number of memory transactions: ";
	cout << (int) get_mem_txns();
	cout << endl;
	*/
	cout << setw(2) << count++ << setw(40) <<". number of cache-to-cache transfers: ";
	cout<<(int)get_cache2cache();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of SignalRds: ";
	cout << (int) get_signal_rds();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of SignalRdXs: ";
	cout << (int) get_signal_rdxs();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of SignalUpgrs ";
	cout << (int) get_signal_upgrs();
	cout << endl;
/*
	cout << setw(2) << count++ << setw(40) <<". number of ResponseReplies ";
	cout << (int) get_response_replies();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of ResponseReplyDs ";
	cout << (int) get_response_replyds();
	cout << endl;
*/
	cout << setw(2) << count++ << setw(40) <<". number of invalidations: ";
	cout << (int) get_invalidations();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of interventions: ";
	cout << (int) get_interventions();
	cout << endl;
/*
	cout << setw(2) << count++ << setw(40) <<". number of Flushes ";
	cout << (int) get_flushes();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of InvAcks ";
	cout << (int) get_inv_acks();
	cout << endl;
	cout << setw(2) << count++ << setw(40) <<". number of Acks ";
	cout << (int) get_acks();
	cout << endl;
*/
}
