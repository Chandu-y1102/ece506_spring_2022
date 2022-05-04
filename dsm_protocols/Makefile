#CC = g++44 -std=c++0x
CC = g++ -std=c++0x
OPT = -O3
WARN = -Wall
ERR = -Werror

CFLAGS = $(OPT) $(WARN) $(ERR)

DSM_SRC = main.cc cache.cc io_function.cc fbv.cc ssci.cc directory.cc mesi.cc
DSM_OBJ = main.o cache.o io_function.o fbv.o ssci.o directory.o mesi.o

all: dsm
	@echo "Compilation Done ---> nothing else to make :) "

dsm: $(DSM_OBJ)
	$(CC) -o dsm $(CFLAGS) $(DSM_OBJ) -lm
	@echo "---------------------------------------------------------------"
	@echo "-----------SP2022-506 DSM SIMULATOR                  ----------"
	@echo "---------------------------------------------------------------"

.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc

clean:
	rm -f *.o dsm
