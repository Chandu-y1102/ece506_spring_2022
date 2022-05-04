/************************************************************
                        Course          :       CSC/ECE506
                        Source          :       msiup.h
                        Instructor      :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
*************************************************************/
#include "cache.h"
class writethrough : public Cache {
 public:
        writethrough(int s, int a, int b, int p , int pn)
                                : Cache(s, a, b, p, pn) {};
        ~writethrough() {};
        void PrRd(ulong addr, int processorNumber);
        void PrWr(ulong addr, int processorNumber);
	void PrRdDir(ulong addr, int processorNumber){;}
        void PrWrDir(ulong addr, int processorNumber){;}
        bool writeback_needed(cache_state state);
        void BusRd(ulong addr);
        void BusRdX(ulong addr);
};
