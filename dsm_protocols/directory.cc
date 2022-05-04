/************************************************************
			Course		: 	CSC/ECE506
			Source 		:	directory.cc
			Owner		:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
*************************************************************/ 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;

#include "directory.h"
#include "main.h"
#include "fbv.h"
#include "ssci.h"

ulong Directory::dir_size = 0;

Directory::Directory(ulong num_entries, int type) {
	invalidations = interventions = 0;
	dir_size = num_entries;

	for (uint i = 0; i < dir_size; i++){
		if (type) {
			entry.push_back(new SSCI());
		}
		else {
			entry.push_back(new FBV());
		}
		entry[i]->tag   = 0;
		entry[i]->state = U;
	}
}

Directory::~Directory() { 
	for (uint i = 0; i < dir_size; i++){
		delete entry[i];
	}
}
//
dir_entry *Directory::find_dir_line(ulong tag) {
	// FIX_ME : Traverse the directory till dir_size
	// check if the tag matches. If yes, return the 
	// directory entry. Else return NULL
	for (uint i = 0; i < dir_size; i++){
                if(entry[i]-> get_dir_tag() == tag)
		return entry[i];
	}

        return NULL;
}
//
dir_entry *Directory::find_empty_line(ulong tag) {
	// FIX_ME : Traverse the directory till dir_size
	// check if there is an uncached line. If yes, return
	// the uncached entry. You may want to pop an error
	// if no empty line and terminate the execution
	for (uint i = 0; i < dir_size; i++){
		if(entry[i]-> get_state() == U){
			entry[i]->set_dir_tag(tag); 
			return entry[i];
        }
	}
	return NULL;
}
//
