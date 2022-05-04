/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	cache.h
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/

#ifndef CACHE_H
#define CACHE_H

#include <cmath>
#include <iostream>
#include "directory.h"

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

class Cache;
typedef void (Cache::*fptr)(ulong);

/****add new states, based on the protocol****/
enum cache_state 
{
	I, // invalid
	E, // exclusive
	M, // modified
	Sc, // shared clean
	Sm, // shared modified
	V, // valid exclusive
	S, // shared
	D, // dirty
};

//
class cache_line {

protected:
   ulong tag;
   cache_state State;   
   ulong seq;
 
public:
   cache_line()						{ tag = 0; State = I; }
   ulong get_tag()       				{ return tag; }
   cache_state get_state()				{ return State;}
   ulong get_seq()       				{ return seq; }
   void set_seq(ulong Seq)				{ seq = Seq;}
   void set_state(cache_state state)			{ State = state;}
   void set_tag(ulong a)				{ tag = a;}
   bool valid()        					{ return ((State) != I); }
};

class Cache {
protected:
	ulong size, line_size, assoc;
	ulong sets, log2_sets, log2_blk;
	ulong tag_mask, num_lines, num_processor;
	ulong reads, read_misses, writes;
	ulong write_misses, write_backs;
	ulong memory_transactions, cache2cache;
	ulong signal_rds, signal_rdxs, signal_upgrs;
	ulong response_replies, response_replyds; 
	ulong invalidations, interventions, flushes, inv_acks, acks;	
	ulong I2E, I2S, I2M, E2M, E2S, E2I, S2M, S2I, M2S, M2I;
	int cache_num, num_processors;									
	cache_line **cache;
	Directory * directory;

  	ulong tag_field(ulong addr)		{ return (addr >> (log2_blk) );}
	ulong index_field(ulong addr)	{ return ((addr >> log2_blk)
													& tag_mask);}
	ulong calc_addr4tag(ulong tag) 	{ return (tag << (log2_blk));}
public:
    ulong current_cycle;  
	ulong bus_reads,bus_readxs,bus_updates,bus_upgrades;
    Cache(int, int, int, int, int);
    virtual ~Cache() { delete cache;}

    cache_line * find_line_to_replace(ulong addr);
    virtual cache_line * allocate_line(ulong) = 0;
    cache_line * find_line(ulong addr);
    cache_line * get_line_to_evict(ulong);

    ulong get_read_misses()		{return read_misses;} 
	ulong get_write_misses()	{return write_misses;} 
    ulong get_reads()			{return reads;}   
	ulong get_writes()			{return writes;}
    ulong get_write_backs()		{return write_backs;}
    ulong get_mem_txns() 		{return memory_transactions;}
    ulong get_cache2cache() 	{return cache2cache;}
	
	//DSM Statistics
	ulong get_signal_rds()		{return signal_rds;}
	ulong get_signal_rdxs()		{return signal_rdxs;}
	ulong get_signal_upgrs()	{return signal_upgrs;}
	ulong get_response_replies(){return response_replies;}
	ulong get_response_replyds(){return response_replyds;}
	ulong get_invalidations() 	{return invalidations;}
	ulong get_interventions() 	{return interventions;}
	ulong get_flushes()			{return flushes;}
	ulong get_inv_acks()		{return inv_acks;}
	ulong get_acks()			{return acks;}

    void write_back(ulong)   	{write_backs++;}
    void print_stats(int, int, int);
    void update_LRU(cache_line *);
	
	// Print Final Statistics for 
	//Directory-Based Shared Multiprocessor
	void set_directory(Directory *dir) {directory = dir;}

	// Methods for Processor Operations
    virtual void PrRd(ulong, int) = 0;
    virtual void PrWr(ulong, int) = 0;
		
	// Methods for Directory-Based Shared Multiprocessor
	virtual void signalRd(ulong, int) 	{return;};
    virtual void signalRdX(ulong, int) 	{return;};
    virtual void signalUpgr(ulong, int)	{return;};
	virtual void NullDirOp(ulong)		{return;};
    virtual void Int(ulong) 			{return;};
    virtual void Inv(ulong) 			{return;};

};

// Abstract Factory Creational Pattern to 
// create the caches based on the protocol

class cache_factory {
public:
    enum protocol_name{
		MESI_dsm
    };
	
	Directory *my_directory;
    static Cache* create_cache(	int protocol_no, int size, int assoc, 
								int block_size, int num_processor, 
								ulong directory_size, int processor_num);
};

#endif
