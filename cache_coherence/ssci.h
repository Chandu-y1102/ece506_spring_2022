/************************************************************
                        Course          :       CSC456
                        Source          :       ssci.h
                        Owner           :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------*
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
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
        int get_OwnerField() {return dir_entry::OwnerField;} // Added this
        void set_dir_state(dir_state d_state) { dir_entry::state = d_state;}
        void set_dir_tag(unsigned long a) { tag = a; }
        void set_OwnerField(int a) {OwnerField = a;} // Added this
        void update_sharer_entry(int proc_num);
        void remove_sharer_entry(int proc_num);
        int check_sharer_entry(int np);
        void sendInv_sharer(ulong addr, int num_proc, int proc_num);
        void sendInt_sharer(ulong addr, int num_proc, int proc_num);
};

