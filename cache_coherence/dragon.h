/************************************************************
                        Course          :       CSC/ECE506
                        Source          :       msi.h
                        Instructor      :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
Code Integration : Tarun Govind Kesavamurthi
*************************************************************/
#include "cache.h"
class Dragon : public Cache {
 public:
        Dragon(int s, int a, int b, int p, int pn):
                Cache(s, a, b, p, pn) {};
        ~Dragon() {};
        void PrRd(ulong addr, int processor_number);
        void PrWr(ulong addr, int processor_number);
	void PrRdDir(ulong addr, int processor_number);
        void PrWrDir(ulong addr, int processor_number);
        bool writeback_needed(cache_state state);
        void BusRd(ulong addr);
        void BusRdX(ulong addr);
        void BusUpgr(ulong addr);
	void signalRd(ulong addr, int processorNumber);
        void signalRdX(ulong addr, int processorNumber);
        void signalUpgr(ulong addr, int processorNumber);
        void Int(ulong addr);
        void Inv(ulong addr);
        cache_line *allocate_line_dir(ulong addr);
		void BusWr(ulong addr);
};

