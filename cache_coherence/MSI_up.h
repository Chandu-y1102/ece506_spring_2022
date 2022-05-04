/************************************************************
                        Course          :       CSC/ECE506
                        Source          :       MSI_up.h
                        Instructor      :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
*************************************************************/
#include "cache.h"
class MSI_up : public Cache {
 public:
        MSI_up(int s, int a, int b, int p, int pn):
                Cache(s, a, b, p, pn) {};
        ~MSI_up() {};
        void PrRd(ulong addr, int processor_number);
        void PrWr(ulong addr, int processor_number);
	void PrRdDir(ulong addr, int processor_number){;}
        void PrWrDir(ulong addr, int processor_number){;}
        bool writeback_needed(cache_state state);
        void BusRd(ulong addr);
        void BusRdX(ulong addr);
	void BusUpgr(ulong addr);
};

