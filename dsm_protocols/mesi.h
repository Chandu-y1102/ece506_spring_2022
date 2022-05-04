/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	mesi.h
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/
class MESI: public Cache {
 public:
	MESI(int s, int a, int b, int p, int pn): Cache(s, a, b, p, pn) {};
	~MESI() { /* insert real code here */};
	void PrRd(ulong addr, int processorNumber);
	void PrWr(ulong addr, int processorNumber);
	void signalRd(ulong addr, int processorNumber);
	void signalRdX(ulong addr, int processorNumber);
	void signalUpgr(ulong addr, int processorNumber);
	void Int(ulong addr);
	void Inv(ulong addr);
	void notifyCtrl(ulong addr);
	cache_line *allocate_line(ulong addr);
};
