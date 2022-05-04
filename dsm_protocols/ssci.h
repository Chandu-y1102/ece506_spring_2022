/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	ssci.h
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/
#include "directory.h"
#include <list>
using namespace std;
class SSCI: public dir_entry {
 private:
	list <int> cache_list;

 public:

	unsigned long get_dir_tag() { return dir_entry::tag; }
	dir_state get_state() { return dir_entry::state;}
	void set_dir_state(dir_state d_state) { dir_entry::state = d_state;}
	void set_dir_tag(unsigned long a) { tag = a; }
	void add_sharer_entry(int proc_num);
	void remove_sharer_entry(int proc_num);
	int is_cached(int np);
	//num_proc - number of processors in the DSM
	//proc_num - processor number invoking the function
	void sendInv_to_sharer(ulong addr, int num_proc, int proc_num);
	void sendInt_to_sharer(ulong addr, int num_proc, int proc_num);
};
